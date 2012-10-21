var A = 0.1;
var C = 0.9993;
var N = 2000;
var MAX_TRIALS = 5000;
var S = undefined;
var i = 0;

function proceed()
{
  A = A * C;
  S = A / 2;
  ++i;
}

function getShape(x0, y0)
{
  var path = new PainterPath;
  var s = S/2;
  if (i % 2 == 0) {
    path.moveTo(x0, y0-s);
    path.lineTo(x0+s, y0+s);
    path.lineTo(x0-s, y0+s);
    path.lineTo(x0, y0-s);
  }
  else {
    path.moveTo(x0, y0+s);
    path.lineTo(x0+s, y0-s);
    path.lineTo(x0-s, y0-s);
    path.lineTo(x0, y0+s);
  }
  return path;
}

function getColor()
{
  return [ i%256, 0, 256-i%256 ];
}

MainWindow.startTiling()
