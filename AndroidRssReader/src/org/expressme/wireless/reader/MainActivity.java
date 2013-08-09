package org.expressme.wireless.reader;

import java.util.List;

import org.expressme.wireless.reader.provider.BriefItem;
import org.expressme.wireless.reader.service.ReadingService;

import android.app.ListActivity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.provider.BaseColumns;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;

/**
 * List all feed items in ListView.
 * 
 * @author Michael Liao (askxuefeng@gmail.com)
 */
public class MainActivity extends ListActivity {

    private final Log log = Utils.getLog(getClass());

    boolean unreadOnly = true;
    RssAdapter itemListAdapter = null;
    ComponentName service = null;

    private ReadingService serviceBinder = null;

    private ServiceConnection serviceConnection = new ServiceConnection() {
            public void onServiceConnected(ComponentName className, IBinder service) {
                serviceBinder = ((ReadingService.ReadingBinder)service).getService();
                unreadOnly = serviceBinder.getPreferenceOfUnreadOnly();
                serviceBinder.loadStoredItem();
            }

            public void onServiceDisconnected(ComponentName className) {
                serviceBinder = null;
            }
    };

    static final int MENU_REFRESH = Menu.FIRST;
    static final int MENU_SUBS = Menu.FIRST + 1;
    static final int MENU_PREF = Menu.FIRST + 2;
    static final int MENU_ALLREAD = Menu.FIRST + 3;
	static final int MENU_ALLDEL = Menu.FIRST + 4;
	static final int MENU_EXIT = Menu.FIRST + 5;


	public static final int requestCode=0x20;

    private final BroadcastReceiver newItemsReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
        	if ((serviceBinder == null)||(itemListAdapter == null))
				return;
            List<BriefItem> list = serviceBinder.queryBriefItems((-1), unreadOnly);
            itemListAdapter.clear();
            for (BriefItem item : list) {
                itemListAdapter.add(item);
            }
        }
    };

    private final BroadcastReceiver subRemovedReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
        	serviceBinder.loadStoredItem();
        }
    };

    private final BroadcastReceiver prefChangedReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (serviceBinder==null)
                return;
            boolean b = serviceBinder.getPreferenceOfUnreadOnly();
            if (b!=unreadOnly) {
                unreadOnly = b;
                serviceBinder.loadStoredItem();
            }
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0, MENU_REFRESH, 0, getResources().getString(R.string.menu_refresh)).setIcon(android.R.drawable.ic_menu_more);
        menu.add(0, MENU_SUBS, 1, getResources().getString(R.string.menu_subs)).setIcon(android.R.drawable.ic_menu_manage);
        menu.add(0, MENU_PREF, 2, getResources().getString(R.string.menu_pref)).setIcon(android.R.drawable.ic_menu_preferences);
        menu.add(0, MENU_ALLREAD, 3, getResources().getString(R.string.mark_all_read)).setIcon(android.R.drawable.ic_menu_save);
        menu.add(0, MENU_ALLDEL, 4, getResources().getString(R.string.del_all_item)).setIcon(android.R.drawable.ic_menu_close_clear_cancel);
        menu.add(0, MENU_EXIT, 5, getResources().getString(R.string.app_exit)).setIcon(android.R.drawable.ic_menu_close_clear_cancel);
		return true;
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        super.onPrepareOptionsMenu(menu);
        //MenuItem item = menu.findItem(MENU_REFRESH);
        //item.setEnabled(hasNewItems);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case MENU_REFRESH:
            refreshItems();
            break;
        case MENU_SUBS:
			startActivityForResult(new Intent(this, SubsActivity.class),requestCode);
            break;
        case MENU_PREF:
			startActivityForResult(new Intent(this, PrefActivity.class),requestCode);
            break;
        case MENU_ALLREAD:
        	markAllItemsRead();
        	break;
		case MENU_ALLDEL:
			serviceBinder.delAllItems();
			break;
		case MENU_EXIT:
			finish();
			break;
        }
        return true;
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        log.info("Position = " + position + ", ID = " + id);
        BriefItem item = (BriefItem) l.getItemAtPosition(position);
        if (item.unread) {
            item.unread = false;
            this.serviceBinder.markRead(item.id);
            if (this.unreadOnly)
                this.itemListAdapter.remove(item);
            else {
                // update view
                this.itemListAdapter.notifyDataSetChanged();
            }
        }
        Intent intent = new Intent(this, ReadActivity.class);
		Bundle bundle=new Bundle();
		bundle.putLong(BaseColumns._ID, item.id);
		bundle.putBoolean("showimg",serviceBinder.getPreferenceOfImage());
		intent.putExtras(bundle);
        startActivityForResult(intent,requestCode);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        setTitle(getResources().getString(R.string.app_name));
        this.itemListAdapter = new RssAdapter(this, R.layout.list_item);
        setListAdapter(this.itemListAdapter);

        // register receiver:
        registerReceiver(newItemsReceiver, new IntentFilter(ReadingService.NOTIFY_NEW_ITEMS));
        registerReceiver(subRemovedReceiver, new IntentFilter(ReadingService.NOTIFY_SUB_REMOVED));
        registerReceiver(prefChangedReceiver, new IntentFilter(ReadingService.NOTIFY_PREF_CHANGED));
        service = startService(new Intent(this, ReadingService.class));

        // bind service:
        Intent bindIntent = new Intent(this, ReadingService.class);
        bindService(bindIntent, serviceConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onDestroy() {
		finishActivity(requestCode);
        super.onDestroy();
        unregisterReceiver(newItemsReceiver);
        unregisterReceiver(subRemovedReceiver);
        unregisterReceiver(prefChangedReceiver);
        unbindService(serviceConnection);
        stopService(new Intent(this, ReadingService.class));
//		android.os.Process.killProcess(android.os.Process.myPid());
//		System.exit(0);
    }

    private void refreshItems() {
    	serviceBinder.updateItemsNow();
    }
    
    private void markAllItemsRead() {
    	serviceBinder.markAllItemsRead();
    	serviceBinder.loadStoredItem();
    }

}
