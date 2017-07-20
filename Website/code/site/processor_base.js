
///////////////////

//var NewsTitles = ['Hey! News title one!', 'News title two!', 'News title three'];
//var NewsDates = ['6/21/2017', '5/10/2017', '1/5/2017'];
//var NewsSample = ['This is what the post is about', 'This post is about something else entirely', 'Here is another post not about anything important at at all'];

// This is for the games page. When adding, always add on the left
var GamesImages = [
'games/Racetrack/Racetrack.png',
'games/CoinCollecter/CoinCollector.png',
'games/Yahtz/Yahtz.png',
'games/Situps/Situps.png',
'games/TowerOfHanoi/TowerOfHanoi.png'
];

var GamesDate = [
'7/18/2017',
'7/17/2017',
'7/11/2017',
'7/6/2017',
'7/5/2017'
];

var GamesName = [
'Racetrack',
'CoinCollector',
'Yahtz',
'Situps',
'Tower of Hanoi'
];

var GamesDescription = [
'Replication of the pen and paper Racetrack game.',
'Moving around a character and picking up coins. Arrays and iteration proof of concept.',
'Collecting coins, moving characters, action points.',
'Moving workers and collecting resources.',
'The classic Tower of Hanoi stacking disk game.'
];

var GamesPageLink = [
'games/Racetrack/Racetrack.html',
'games/CoinCollecter/CoinCollector.html',
'games/Yahtz/Yahtz.html',
'games/Situps/Situps.html',
'games/TowerOfHanoi/TowerOfHanoi.html'
];
// ----



function PageLoad(pageName, locationPrefix) {
    document.getElementById("header").innerHTML = PrefixNavbar(locationPrefix);

  if (pageName == "Index") {    
    document.getElementById("PostOne").innerHTML = GetPostHTML(0);
    document.getElementById("PostTwo").innerHTML = GetPostHTML(1); 

    document.getElementById("GameOne").innerHTML = GetGameIndexHTML(0); 
    document.getElementById("GameTwo").innerHTML = GetGameIndexHTML(1); 

  } else if (pageName == "News") {
    for (index = 0; index < NewsTitles.length; index++) {
      document.getElementById("posts").innerHTML += GetNewsPostHTML(index);
    }
  } else if (pageName == "Games") {
    for (index = 0; index < GamesName.length; index++) {
      document.getElementById("GamesHTML").innerHTML += GetGamesPageHTML(index);
    }
  }
}

function PrefixNavbar(prefix) {
  var finalHTML = "";
  finalHTML += "<div class='container'>";
  finalHTML += "<div class='row'>";
  finalHTML += "<table class='u-full-width'><thead><tr>";
  finalHTML += "<th><a href='" + prefix + "index.html'><img src='" + prefix + "images/cathead.png' width='35'></a></th>"
  finalHTML += "<th><a href='" + prefix + "news.html'>News</a></th>"
  finalHTML += "<th><a href='" + prefix + "games.html'>Games</a></th>"
  finalHTML += "<th><a href='" + prefix + "download.html'>Download</a></th>"
  finalHTML += "<th><a href='" + prefix + "about.html'>About</a></th>"
  finalHTML += "</tr></thead></table></div></div></div>";
  return (finalHTML);
}

function GetGameIndexHTML(index) {
  var finalHTML = "";
  finalHTML += '<img class="u-max-full-width" src="' + GamesImages[index] + '">';
  finalHTML += '<div class="clearfix mxn2"><div class="sm-col sm-col-6 md-col-5 lg-col-6 px2">'
  finalHTML += '<h5><strong><a href="' + GamesPageLink[index] + '">' + GamesName[index] + '</a></strong></h5></div>';
  finalHTML += '<div class="sm-col sm-col-6 md-col-7 lg-col-6 px2">'
  finalHTML += '<p>' + GamesDescription[index] + '</p></div></div>';
  return (finalHTML);
}

function GetPostHTML(index) {

  // title
  var finalHTML = "";
  finalHTML += '<h5><strong><a href="';
  finalHTML += NewsTitles[index] + '.html"';
  finalHTML += '>';
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
  finalHTML += '<pre></pre>';
  finalHTML += '<div class="container">';
  finalHTML += '<div class="row">';
  finalHTML += '<div class="clearfix mxn2">';
  finalHTML += '<div class="sm-col sm-col-6 md-col-5 lg-col-4 px2">';
  finalHTML += '<h5><a href="';
  finalHTML += NewsTitles[index] + '.html"';
  finalHTML += "><strong>";
  finalHTML += NewsTitles[index];
  finalHTML += "</strong></a></h5>";
  finalHTML += "<h6>";
  finalHTML += NewsDates[index];
  finalHTML += "</h6>";
  finalHTML += "</div>";
  finalHTML += '<div class="sm-col sm-col-6 md-col-7 lg-col-8 px2">';
  finalHTML += '<p>';
  finalHTML += NewsSample[index];
  finalHTML += '</p>';
  finalHTML += '</div>';
  finalHTML += '</div>';
  finalHTML += '</div>';
  finalHTML += '</div>';
  return (finalHTML);
}

function GetGamesPageHTML(index) {
  var finalHTML = "";
  finalHTML += '<div class="container">';
  finalHTML += '<div class="row">';
  finalHTML += '<div class="three columns">';
  finalHTML += '<h5><strong><a href="';
  finalHTML += GamesPageLink[index];
  finalHTML += '">';
  finalHTML += GamesName[index];
  finalHTML += "</strong></a></h5>";
  finalHTML += '<h6>';
  finalHTML += GamesDate[index];
  finalHTML += '</h6>';
  finalHTML += '<p>';
  finalHTML += GamesDescription[index];
  finalHTML += '</p>';
  finalHTML += '<p></p>';
  finalHTML += '</div>';
  finalHTML += '<div class="nine columns">';
  finalHTML += '<a href="';
  finalHTML += GamesPageLink[index];
  finalHTML += '">';
  finalHTML += "<img class='u-max-full-width' src='";
  finalHTML += GamesImages[index];
  finalHTML += "'>";
  finalHTML += "</a>";
  finalHTML += '<p></p>';
  finalHTML += '</div></div></div>';
  return (finalHTML);
}