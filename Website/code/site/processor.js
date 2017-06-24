var NewsTitles = ['Update One','Update Two'];var NewsDates = ['4/3/2017','4/22/2017'];var NewsSample = ['Sample goes here. Sample goes here. Sample goes here. Sample goes here. Sample goes here.','Sample goes here. Sample goes here. Sample goes here. Sample goes here. Sample goes here.'];
///////////////////

//var NewsTitles = ['Hey! News title one!', 'News title two!', 'News title three'];
//var NewsDates = ['6/21/2017', '5/10/2017', '1/5/2017'];
//var NewsSample = ['This is what the post is about', 'This post is about something else entirely', 'Here is another post not about anything important at at all'];


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
    for (index = 0; index < NewsTitles.length; index++) {
      document.getElementById("posts").innerHTML += GetNewsPostHTML(index);
    }
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

function GetNewsPostHTML(index) {

  var finalHTML = "";
  finalHTML+= '<pre></pre>';
  finalHTML+= '<div class="container">';
  finalHTML+= '<div class="row">';
  finalHTML+= '<div class="clearfix mxn2">';
  finalHTML+= '<div class="sm-col sm-col-6 md-col-5 lg-col-4 px2">';
  finalHTML+= "<h5><a href='http://google.com'><strong>";
  finalHTML+= NewsTitles[index];
  finalHTML+= "</strong></a></h5>";
  finalHTML+= "<h6>";
  finalHTML+= NewsDates[index];
  finalHTML+= "</h6>";
  finalHTML+= "</div>";
  finalHTML+= '<div class="sm-col sm-col-6 md-col-7 lg-col-8 px2">';
  finalHTML+= '<p>';
  finalHTML+= NewsSample[index];
  finalHTML+= '</p>';
  finalHTML+= '</div>';
  finalHTML+= '</div>';
  finalHTML+= '</div>';
  finalHTML+= '</div>';
  return (finalHTML);
}