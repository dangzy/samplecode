import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JPanel;
import javax.swing.BoxLayout;
import javax.swing.JTree;
import javax.swing.JTable;
import javax.swing.JTextPane;
import javax.swing.JButton;
import java.net.URL;
import java.net.URLConnection;
import java.io.InputStreamReader;
import java.awt.Container;

import view.button;
import control.feedmanager;

class MainFrame extends JFrame {
    public MainFrame() {
        //create main frame
        super();
        setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
        Container coner=this.getContentPane();
        setLayout(new BoxLayout(coner,BoxLayout.X_AXIS));

        //create feedmanager
        feedmanager myfeeds=new feedmanager();

        //create sidebar
        JPanel pSide=new JPanel();
        BoxLayout mylay=new BoxLayout(pSide,BoxLayout.Y_AXIS);
        pSide.setLayout(mylay);
        //add addfeed button
        m_btAddFeed=new button("AddFeed",myfeeds);
        pSide.add(m_btAddFeed);
        //TODO:add feeds tree
        coner.add(pSide);
        
        //create main content
        JPanel pCon=new JPanel();
        BoxLayout mylay2=new BoxLayout(pCon,BoxLayout.Y_AXIS);
        pCon.setLayout(mylay2);
        //add discription
        m_tpDesc=new JTextPane();
        pCon.add(m_tpDesc);
        //TODO:add titles
        coner.add(pCon);

        
        //add debug text area
        JTextArea debugtext=new JTextArea(10,80);
        debugtext.setLineWrap(true);
        JScrollPane span=new JScrollPane(debugtext);
        coner.add(span);

        setVisible(true);

        //test code
        try {
            URL testl = new URL("http://cnbeta.feedsportal.com/c/34306/f/624776/index.rss");
            URLConnection tcon=testl.openConnection();
            tcon.setRequestProperty("User-Agent","Opera/9.80 (Windows NT 5.1; Edition IBIS) Presto/2.12.388 Version/12.14");
            InputStreamReader xmlin=new InputStreamReader(tcon.getInputStream(),"utf-8");
            int MAXLEN = 1024;
            char[] b =new char[MAXLEN];
            int offset = 0, rdlen = 0;
            while (-1!=(rdlen = xmlin.read(b,0,MAXLEN))) {
                debugtext.append(new String(b));
                offset += rdlen;
            }
            xmlin.close();
        }catch(Exception e) {
            System.out.println(e.toString());
        }
        pack();
    }
    public JButton m_btAddFeed;
    public JTree m_trFeeds;
    public JTable m_tbItems;
    public JTextPane m_tpDesc;

}

public class load {
    public static void main(String[] args) {
        MainFrame mframe = new MainFrame();
    }
}
