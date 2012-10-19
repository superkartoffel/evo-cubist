A = 0.1;
C = 0.9993;
N = 2000;
MAX_TRIALS = 5000;
S = undefined;

function proceed()
{
  A = A * C;
  S = A / 2;
}

function getShape(x0, y0)
{
  var path = new PainterPath;
  path.addEllipse(x0-S/2, y0-S/2, S, S);
  return path;
}

MainWindow.startTiling()
