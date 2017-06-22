
var NewsTitles = ['Hey! News title one!', 'News title two!', 'News title three'];
var NewsDates = ['6/21/2017', '5/10/2017', '1/5/2017'];
var NewsSample = ['This is what the post is about', 'This post is about something else entirely', 'Here is another post not about anything important at at all'];

///////////////////

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


function PageLoad(pageName) {
    document.getElementById("header").innerHTML = headerHTML;

  if (pageName == "Index") {    
    document.getElementById("PostOne").innerHTML = GetPostHTML(0);
    document.getElementById("PostTwo").innerHTML = GetPostHTML(1);    
  } else if (pageName == "News") {
    console.log("NEWS");
  }
}

function GetPostHTML(index) {

  // title
  var finalHTML = "";
  finalHTML += "<h5><strong><a href='http://google.com'>"
  finalHTML += NewsTitles[index];
  finalHTML += "</a></strong></h5>";

  // sample
  finalHTML += "<p>";
  finalHTML += NewsSample[index];
  finalHTML += "</p>";

  return (finalHTML);
}

function DoNewsPage() {

}
