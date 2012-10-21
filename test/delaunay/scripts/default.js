var A = 0.1;
var C = 0.9993;
var N = 2000;
var MAX_TRIALS = 5000;
var S = 0;
var i = 0;

function proceed()
{
  A = A * C;
  S = A / 2;
  ++i;
}

MainWindow.startTiling()
