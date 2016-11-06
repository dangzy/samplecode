#Dependence
Compilation depend on:

* [tinyxml2](https://github.com/leethomason/tinyxml2)

Running depend on:

* [curl](https://github.com/curl/curl)

rss.exe is precompiled binary for Windows.

#Compile
<code>
g++ rss.cpp -ltinyxml2 -s -o rss
</code>

#Usage
<code>
./rss
</code>
or
<code>
./rss rss.xml
</code>
