#include <fstream>
#include <iostream>
#include <vector>
#include <tinyxml2.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

using namespace std;
namespace xml=tinyxml2;

static vector<string> getxmls(const char* cfg) {
    vector<string> xmls;
    xml::XMLDocument doc;
    doc.LoadFile(cfg);
    xml::XMLElement *e = doc.FirstChildElement("rss");
    while(e) {
        xmls.push_back(e->GetText());
        e = e->NextSiblingElement();
    }
    return xmls;
}

int main(int argc, char *argv[]) {
    if (argc > 2){
        cerr << "Param err" << endl;
        return -1;
    }
    vector<string> xmls = getxmls(argc==1?"rss.xml":argv[1]);
    if (0 == xmls.size()) {
        cerr << "No xml" << endl;
        return -2;
    }
    bool showdetail = false;
    bool iswin = getenv("WINDIR") != NULL?true:false;
    char url[1024] = {0};
    const char* tmpfile="tmp.xml";
    const char* mdfile="rss.html";
    int rc = -1;
    string htmlfile = (iswin?getenv("TMP"):"/tmp")+string("/")+mdfile;
    ofstream md(htmlfile.c_str());
    md<< "<html><head><meta charset=\"utf-8\"></head><body>" << endl;
    int cnt = 0;
    for(int i=0;i < xmls.size();i++) {
        sprintf(url,"curl -s -o %s %s",tmpfile,xmls[i].c_str());
        rc = system(url);
        if (rc == 0) {
            xml::XMLDocument doc;
            doc.LoadFile(tmpfile);
            xml::XMLElement *e = doc.FirstChildElement("rss");
            if (e) {
                e = e->FirstChildElement("channel");
                xml::XMLElement *channel = e->FirstChildElement("title");
                md << "<h1>" << channel->GetText() << "</h1>" <<endl;
                e = e->FirstChildElement("item");
                while (e) {
                    xml::XMLElement *item = e->FirstChildElement("title");
                    if (item){
                        cnt++;
                        md << "<a href=\"" << e->FirstChildElement("link")->GetText() << "\" target=\"_blank\">"<< item->GetText() << "</a><br>" << endl;
                    }
                    if(showdetail) {
                        item = e->FirstChildElement("description");
                        if (item){
                            md << "<div>" << item->GetText() << "</div><br>" << endl;
                        }
                    } else
                        md << "<br>" << endl;
                    e = e->NextSiblingElement();
                }
            } else {
                e = doc.FirstChildElement("feed");
                if (e) {
                    e = e->FirstChildElement("title");
                    md << "<h1>" << e->GetText() << "</h1>" <<endl;
                    e = doc.FirstChildElement("feed")->FirstChildElement("entry");
                    while(e) {
                        xml::XMLElement *item = e->FirstChildElement("title");
                        if (item){
                            cnt++;
                            md << "<a href=\"" << e->FirstChildElement("id")->GetText() << "\" target=\"_blank\">"<< item->GetText() << "</a><br>"<< endl;
                        }
                        if(showdetail){
                            item = e->FirstChildElement("summary");
                            if (item){
                                md << "<div>" << item->GetText() << "</div><br>" << endl;
                            }
                        } else
                            md << "<br>" << endl;
                        e = e->NextSiblingElement();
                    }
                } else
                    cerr << "don't support " << xmls[i] << endl;
            }
            unlink(tmpfile);
        } else
            cerr << "curl err: " << url << endl;
    }
    md << "</body></html>";
    md.close();
    if (iswin)
        sprintf(url,"start %s",htmlfile.c_str());
    else
        sprintf(url,"x-www-browser %s",htmlfile.c_str());
    system(url);
    return 0;
}
