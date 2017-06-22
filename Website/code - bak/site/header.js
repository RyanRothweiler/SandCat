var headerHTML = " \
  <div class='container'> \
    <div class='row'> \
    <table class='u-full-width'> \
      <thead> \
        <tr> \
          <th><a href='index.html'><img src='images/cathead.png' width='35'></a></th> \
          <th><a href='news.html'>News</a></th> \
          <th><a href='http://google.com'>Games</a></th> \
          <th><a href='http://google.com'>Download</a></th> \
          <th><a href='http://google.com'>About</a></th> \
        </tr> \
      </thead> \
    </table> \
    </div> \
    </div> \
  </div> \
";


function InsertNav() {
	document.getElementById("header").innerHTML = headerHTML;
}