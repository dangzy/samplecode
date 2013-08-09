package org.expressme.wireless.reader;

import org.expressme.wireless.reader.provider.ItemColumns;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.BaseColumns;
import android.view.Menu;
import android.view.MenuItem;
import android.webkit.WebView;
import android.webkit.WebViewClient;

/**
 * Show item's full content in WebView.
 * 
 * @author Michael Liao (askxuefeng@gmail.com)
 */
public class ReadActivity extends Activity {

    static final int MENU_MORE = Menu.FIRST;

    static final String CSS;
    static {
        CSS = new StringBuilder(256)
                .append("<style type=\"text/css\">\n")
                .append("<!--\n")
                .append("body { color: #000; background-color: #FFF; }\n")
                .append("a:link,a:visited { color:#09F; }\n")
                .append("a:active,a:hover { color:#F60; }\n")
                .append("-->\n")
                .append("</style>\n")
                .toString();
    }

    private final Log log = Utils.getLog(getClass());
    private String url = null;

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0, MENU_MORE, 0, getResources().getString(R.string.menu_orig_link)).setIcon(android.R.drawable.ic_menu_more);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId()==MENU_MORE) {
            Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
            startActivity(intent);
        }
        return true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.read);

        Intent intent = getIntent();
		Bundle bundle=intent.getExtras();
		long id = -1;
		id = bundle.getLong(BaseColumns._ID);
		boolean shownetimg = true;
		shownetimg=bundle.getBoolean("showimg");
        Cursor cursor = getContentResolver().query(ItemColumns.URI, ItemColumns.ALL_COLUMNS, ItemColumns._ID + "=" + id, null, null);
        if (!cursor.moveToFirst()) {
            show404();
            return;
        }
        url = cursor.getString(cursor.getColumnIndex(ItemColumns.URL));
        String title = cursor.getString(cursor.getColumnIndex(ItemColumns.TITLE));
        String content = cursor.getString(cursor.getColumnIndex(ItemColumns.CONTENT));
        cursor.close();

        // set title:
        setTitle(title);

        // load html:
        StringBuilder html = new StringBuilder(content.length()+200);
        html.append("<html><head><title>")
            .append(title)
            .append("</title>\n")
            .append(CSS)
            .append("</head><body>")
            .append(content)
            .append("</body></html>");
        String baseUrl = getBaseUrl(url);
        log.info(url);
        log.info("base url:" + baseUrl);

        WebView web = (WebView) this.findViewById(R.id.webview);
		web.getSettings().setBlockNetworkImage(!shownetimg);
        web.getSettings().setJavaScriptEnabled(true);
        web.setWebViewClient(new WebViewClient() {
            @Override
            public void onLoadResource(WebView view, String url) {
                super.onLoadResource(view, url);
                log.info("onLoadResource: " + url);
            }
        });
        web.loadDataWithBaseURL(
                url,
                html.toString(),
                "text/html",
                "UTF-8",
                null
        );
    }

    void show404() {
        WebView web = (WebView) this.findViewById(R.id.webview);
        web.loadData(
                "<html><body><h1>404 Not Found</h1><p>The item was deleted.</p></body></html>",
                "text/html",
                "UTF-8"
        );
    }

    String getBaseUrl(String url) {
        int n = url.lastIndexOf('/');
        if (n > "https://".length()) {
            return url.substring(0, n+1);
        }
        return url + "/";
    }

}
