function fnetPrintHeader(title){
	document.write(' \
		<div id="fnet_banner_wrapper">\
			<div id="fnet_banner_logo">\
			<a href="http://fnet.sf.net" target="_blank">\
			<img src="/fnet_logo.gif" border="0" alt="FNET"/>\
 			</a>\
			</div>\
			<div id="fnet_banner_center" nowrap>\
				<div id="fnet_banner_topbox" nowrap>\
					<div class="fnet_banner_title" nowrap>FNET\
						<span class="fnet_banner_internal"><a href="http://fnet.sf.net/">Embedded TCP/IP stack</a></span>\
					</div>\
				</div>\
				<div id="fnet_banner_bottombox">\
					<div class="fnet_banner_bottombg">\
					</div>\
				</div>\
			</div>\
		</div>');
   	fnetPrintMenu();
	document.write(' \
		<div id="fnet_main_content" class="fnet_content_frame">\
		<div class="fnet_hr">\
			<hr/>\
		</div>\
		<h1>\
			<strong>', title, '</strong>\
		</h1>\
		<div class="fnet_hr">\
			<hr/>\
		</div>\
		<div class="fnet_padding">\
		</div>');
}
function fnetPrintMenu(){
	document.write('\
		&nbsp;\
		<div id="fnet_menu_content">\
			<ul id="fnet_menu">\
				<li><a href="/index.html">FNET Overview</a></li>\
				<li><a href="/fnet_set.shtml">FNET Settings</a></li>\
				<li><a href="/fnet_stat.html">FNET Statistics</a></li>\
				<li><a href="/fnet_sup.html">FNET Support</a></li>\
				<li><a>FNET Demos...</a>\
				<ul>\
					<li><a href="/fnet_graph.html">Graph/AJAX Demo</a></li>\
				</ul>\
				</li>\
			</ul>\
		</div>');
}
function fnetPrintFooter(){
	document.write(' \
		</div>\
		<br/>\
		<div id="fnet_footer">\
			<center>&copy; 2005-2013 by Andrey Butok, FNET Community.</center>\
		</div>');
}
function sfHover(){
	var navthree = document.getElementById("fnet_menu");
	if (navthree){
		var sfEls = document.getElementById("fnet_menu").getElementsByTagName("li");
		for (var i=0; i<sfEls.length; i++) {
			sfEls[i].onmouseover=function() {
				this.className+=" sfhover";
			}
			sfEls[i].onmouseout=function() {
				this.className=this.className.replace(new RegExp(" sfhover\\b"),"");
			}
		}
	}
}
if (window.attachEvent)
	window.attachEvent("onload",sfHover);
function fnetXMLHttpRequest(){
	var xmlHTTP = false;
	if (window.ActiveXObject)
		xmlHTTP = new ActiveXObject("Microsoft.XMLHTTP");
	else if (window.XMLHttpRequest)
		xmlHTTP = new XMLHttpRequest();
	if (!xmlHTTP)
		alert("Cannot create an XMLHTTP instance!");
	return xmlHTTP;
}