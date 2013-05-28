package view;
import javax.swing.JButton;
import control.feedmanager;

public class button extends JButton {
    public button(String name, feedmanager mgr) {
        super(name);
        mgr=m_mgr;
    }
    public feedmanager m_mgr;
}
