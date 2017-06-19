var headerHTML = "<div class='clearfix mxn2'> \
  <div class='mx-auto max-width-3'> \
\
	<div class='mx2 border-bottom border-blue flex flex-wrap'>\
  		<div class='pr2'><a href='index.html'><img src='images/cathead.png' width='50'></a></div> \
  		<div class='pr2'><p class='h4 bold'><a href='news.html'>News</a></p></div>\
  		<div class='pr2'><p class='h4 bold'><a href='http://google.com'>Games</a></p></div>\
  		<div class='pr2'><p class='h4 bold'><a href='http://google.com'>Download</a></p></div>\
  		<div class='pr2'><p class='h4 bold'><a href='http://google.com'>About</a></p></div>\
	</div>\
</div>\
</div>\
";

var footerHTML = "<div class='mx2 py4 border-bottom border-blue flex flex-wrap'></div>";

function InsertNav() {
	document.getElementById("header").innerHTML = headerHTML;
	document.getElementById("footer").innerHTML = footerHTML;
}