FNET Doxygen Documentation 
--------------------------

The FNET Documentation is built automatically from the project source code and service files:
- fnet\fnet_doc\doxygen\fnet_gen_doc.in = Doxygen configuration file.
- fnet\fnet_doc\doxygen\fnet_doc.dox = Doxygen documentation framework.
- fnet\fnet_doc\doxygen\fnet_layout.xml = Doxygen layout file.

To generate the documentation on your own, please install the following tools: 
- Doxygen 1.8.3.1:@n http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc
- Microsoft HTML Help Workshop (if you want to get CHM documentation):
 http://download.microsoft.com/download/0/a/9/0a939ef6-e31c-430f-a3df-dfae7960d564/htmlhelp.exe

To generate the HTML/CHM documentation you should run @c fnet_doc\doxygen\fnet_gen_doc.bat file.