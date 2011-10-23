(* ::Package:: *)

ImageFile=False;
If[ImageFile,
  imagesize=500; textsize=14,
  imagesize=Medium; textsize=10];


(* ::Subsection::Closed:: *)
(*Graphics Utility Functions*)


\[Epsilon]=0.02;
{xmin=-4, xmax=4, ymin=-4, ymax=4};


ShowText[text_, x_, y_] :=
  Sequence[Black, Text[Style[text,textsize], {x,y}]]

RotateText[text_, x_, y_] :=
  Sequence[Black, Rotate[Text[Style[text,textsize], {x,y}], -90 Degree]]

RotateText[text_, x_, y_, angle_] :=
  Sequence[Black, Rotate[Text[Style[text,textsize], {x,y}], angle]]


LabeledRectangle[text_, color_, {x1_, y1_}, {x2_, y2_}] :=
  LabeledRectangle[text, color, x1, y1, x2, y2, (x1+x2)/2, (y1+y2)/2]

LabeledRectangle[text_, color_, {x1_, y1_}, {x2_, y2_}, y3_] :=
  LabeledRectangle[text, color, x1, y1, x2, y2, (x1+x2)/2, y3]

LabeledRectangle[text_, color_, {x1_, y1_}, {x2_, y2_}, {x3_, y3_}] :=
  Sequence[color, Rectangle[{x1,y1},{x2,y2}], ShowText[text, x3, y3]]

LabeledRectangle[text_, color_, x1_, y1_, x2_, y2_] :=
  LabeledRectangle[text, color, x1, y1, x2, y2, (x1+x2)/2, (y1+y2)/2]

LabeledRectangle[text_, color_, x1_, y1_, x2_, y2_, y3_] :=
  LabeledRectangle[text, color, x1, y1, x2, y2, (x1+x2)/2, y3]

LabeledRectangle[text_, color_, x1_, y1_, x2_, y2_, x3_, y3_] :=
  Sequence[color, Rectangle[{x1,y1},{x2,y2}], ShowText[text, x3, y3]]


LabeledDisk[text_, x_, y_] :=
  Sequence[Cyan, Disk[{x,y},Scaled[1/60]], Black, Text[text,{x+If[ImageFile,0,\[Epsilon]/2],y}]]

BlackDisk[x_, y_] :=
  Sequence[Black, Disk[{x,y},Scaled[1/120]]]

TerminalRule[text_, x_, y_] :=
  Sequence[Lighter[Red,0.3], Disk[{x,y},Scaled[1/60]], White, Text[text,{x+If[ImageFile,0,\[Epsilon]/2],y}]]

DashedLine[{x1_, y1_}, {x2_, y2_}] :=
  Sequence[Thickness[0.007], Black, Dashing[{0.003,0.02}], Line[{{x1,y1},{x2,y2}}], Dashing[{}]]

DashedLine[x1_, y1_, x2_, y2_] :=
  DashedLine[{x1, y1}, {x2, y2}]


(* ::Subsection::Closed:: *)
(*Domain Map of Integration Rules for (sin(x)^j)^m (a+b sin(x)^k)^n when a^2!=b^2*)


Graphics[{
  LabeledRectangle["Rule 1a
\[LowerRightArrow]\[DownArrow]\[DownArrow]\[Diamond]", Red, xmin, 2, -1, ymax],

  LabeledRectangle["Rule 1b \[LowerRightArrow]\[DownArrow]\[Diamond]", Lighter[Red,0.4], xmin, 1, -1, 2],

  LabeledRectangle["Rule 1c \[LowerRightArrow]\[Diamond]", Lighter[Red,0.7], {xmin, 0}, {-1, 1}, 0.6],

  LabeledRectangle["Rule 2a
\[DownArrow]\[DownArrow]\[DownArrow]\[Diamond]", Lighter[Blue,0.5], -1, 2, 0, ymax],
  DashedLine[-1+\[Epsilon],ymax-\[Epsilon],-1+\[Epsilon],2+\[Epsilon]],

  LabeledRectangle["Rule 2b
\[LowerLeftArrow]\[DownArrow]\[Diamond]", Lighter[Blue,0.8], {0, 1}, {1, ymax}, (ymax+2)/2],

  LabeledRectangle["Rule 3a
\[LeftArrow]\[LeftArrow]\[LeftArrow]\[Diamond]", Green, {2, -1}, {xmax, ymax}, {(xmax+2)/2, (ymax+2)/2}],
  DashedLine[{2+\[Epsilon], -1+\[Epsilon]}, {xmax-\[Epsilon], -1+\[Epsilon]}],

  LabeledRectangle["Rule 3b
\[LowerLeftArrow]\[LeftArrow]\[Diamond]", Lighter[Green,0.7], {1, 0}, {2, ymax}, (ymax+2)/2],

  LabeledRectangle["Rule 4a
\[UpperLeftArrow]\[LeftArrow]\[LeftArrow]\[Diamond]", Pink, {2, ymin}, {xmax, -1}, -2.6],

  LabeledRectangle["Rule 4b
\[UpperLeftArrow]\[LeftArrow]\[Diamond]", Lighter[Pink,0.4], {1, ymin}, {2, -1}, -2.6],

  LabeledRectangle["Rule 5a
\[UpperLeftArrow]\[Diamond]", Lighter[Magenta,0.7], 0, ymin, 1, -1, -2.6],

  LabeledRectangle["Rule 5b
\[UpArrow]\[Diamond]", Lighter[Magenta,0.3], -1, ymin, 0, -1, -2.6],
  DashedLine[-1+\[Epsilon],-1-\[Epsilon],-1+\[Epsilon],ymin+\[Epsilon]],

  LabeledRectangle["Rule 6
\[RightArrow]\[Diamond]", Orange, xmin, ymin, -1, 0, -2.6],

  Thick, Green, Line[{{0,ymin+\[Epsilon]},{0,-1}}],
  RotateText["  Rule 7\[LeftArrow]\[LeftArrow]", 0.15, -3.25],

  Thick, Red, Line[{{0,1},{0,ymax-\[Epsilon]}}],
  RotateText["  Rule  8\[RightArrow]\[RightArrow]", 0.15, 2],

  Thick, Green, Line[{{xmin+\[Epsilon],0},{-1,0}}],
  ShowText["Rule 9 \[RightArrow]\[Diamond]", (xmin-1)/2, 0.13],

  Thick, Red, Line[{{1,0},{xmax-\[Epsilon],0}}],
  ShowText["Rule 10 \[LeftArrow]\[LeftArrow]\[Diamond]", (xmax+2)/2, 0.13],

  Thick, Lighter[Blue,0.5], Line[{{2,-1},{2,ymin+\[Epsilon]}}],
  RotateText[" Rule 11\[LowerLeftArrow]", 2.15, -3.5],

  Thick, Blue, Line[{{2,-1},{2,ymax-\[Epsilon]}}],
  RotateText["Rule  12 \[DownArrow]", 2.15, 2],

(*Thick, Blue, Line[{{3/2,-1},{3/2,1}}],
  RotateText["Rule  13\[DownArrow]\[Diamond]", 1.63, 0], *)

  Thick, Black, Line[{{xmin+\[Epsilon],1},{xmax-\[Epsilon],1}}],
  Thick, Black, Line[{{1,ymax-\[Epsilon]},{1,ymin+\[Epsilon]}}],

  Thick, Black, Line[{{xmin+\[Epsilon],2},{xmax-\[Epsilon],2}}],

  TerminalRule["a",0,1],
  TerminalRule["a",0,-1],
  TerminalRule["b",0,2],
  TerminalRule["b",0,-2],

  TerminalRule["c",-1,0],
  LabeledDisk["d",2,0],
  TerminalRule["e",1/2,0],
  TerminalRule["f",-1/2,0],
  TerminalRule["e",0,1/2],
  TerminalRule["f",0,-1/2],

  LabeledDisk["g",-1,-1],
  TerminalRule["h",-1,-1/2],
  TerminalRule["h",-1/2,-1],

  LabeledDisk["i",1/2,-1],
  LabeledDisk["i",-1,1/2],
  LabeledDisk["j",3/2,-1],
  LabeledDisk["j",-1,3/2],

  TerminalRule["k",-1/2,-1/2],
  LabeledDisk["l",1/2,1/2],
  LabeledDisk["m",1/2,-1/2],
  LabeledDisk["m",-1/2,1/2],

  LabeledDisk["n",1/2,-3/2],
  LabeledDisk["n",-3/2,1/2],
  LabeledDisk["o",3/2,-1/2],
  LabeledDisk["o",-1/2,3/2],
  LabeledDisk["p",3/2,-3/2],
  LabeledDisk["p",-3/2,3/2],
  LabeledDisk["q",-1/2,-3/2],
  LabeledDisk["q",-3/2,-1/2]
},
ImageSize->imagesize,
Axes->True,AxesLabel->{n,j k m}
]


(* ::Subsection::Closed:: *)
(*Domain Map of Integration Rules for (sin(x)^j)^m (A+B sin(x)^k) (a+b sin(x)^k)^n when a^2!=b^2*)


Graphics[{
  LabeledRectangle["Rule 1a
\[LowerRightArrow]\[DownArrow]\[Diamond]", Lighter[Red,0.3], {xmin, 1}, {-1, ymax}, {(xmin-1)/2, (ymax+1)/2}],

  LabeledRectangle["Rule 1b \[LowerRightArrow]\[Diamond]", Lighter[Red,0.5], {xmin, 0}, {-1, 1}, {(xmin-1)/2, 1/2}],
  Thick, White, Line[{{-1,0},{-1,1}}],

  LabeledRectangle["Rule 2
\[DownArrow]\[DownArrow]\[Diamond] ", Lighter[Blue,0.5], {-1, 1}, {0, ymax}, (ymax+1)/2],
  DashedLine[-1+\[Epsilon], ymax-\[Epsilon], -1+\[Epsilon], 1],

  LabeledRectangle["Rule 3a
\[LeftArrow]\[LeftArrow]\[Diamond]", Lighter[Green,0.2], {1, -1}, {xmax, ymax}, (ymax+1)/2],
  DashedLine[{1+\[Epsilon], -1+\[Epsilon]}, {xmax-\[Epsilon], -1+\[Epsilon]}],

  LabeledRectangle["Rule 3b
\[LowerLeftArrow]\[Diamond]", Lighter[Green,0.7], {0, 0}, {1, ymax}, (ymax+1)/2],

  LabeledRectangle["Rule 4a
\[UpperLeftArrow]\[LeftArrow]\[Diamond]", Pink, {1, ymin}, {xmax, -1}, (ymin-1)/2],

  LabeledRectangle["Rule 4b
\[UpperLeftArrow]\[Diamond]", Lighter[Pink,0.6], {0, ymin}, {1, -1}, (ymin-1)/2],

  LabeledRectangle["Rule 5
\[UpArrow]\[Diamond]", Magenta, {-1, ymin}, {0, -1-\[Epsilon]}, (ymin-1)/2],
  DashedLine[-1+\[Epsilon],-1-2\[Epsilon],-1+\[Epsilon],ymin+\[Epsilon]],

  LabeledRectangle["Rule 6
\[RightArrow]\[Diamond]", Orange, {xmin, ymin}, {-1, 0}, {(xmin-1)/2, (ymin-1)/2}],
  Thick, White, Line[{{-1,0-\[Epsilon]},{-1,-1}}],

  Thick, Lighter[Green,0.2], Line[{{0, ymin+\[Epsilon]}, {0, -1-\[Epsilon]}}],
  RotateText["Rule 7 \[LeftArrow]", 0.15, -3.25],

  Thick, Blue, Line[{{0, 0}, {0, ymax-\[Epsilon]}}],
  RotateText["Rule 8 \[RightArrow]", 0.15, 1.7],

  Thick, Lighter[Green,0.2], Line[{{xmin+\[Epsilon], 0}, {-1, 0}}],
  ShowText["Rule 9 \[RightArrow]", (xmin-1)/2, 0.13],

  Thick, Blue, Line[{{\[Epsilon], 0}, {xmax-\[Epsilon], 0}}],
  ShowText["Rule 10 \[LeftArrow]", (xmax+1)/2, 0.13],

  Thick, Yellow, Line[{{1,-1-\[Epsilon]},{1,ymin+\[Epsilon]}}],
  RotateText["Rule 11 \[LowerLeftArrow]", 1.15, (ymin-1)/2],

  Thick, Lighter[Magenta,0.5], Line[{{1,ymax-\[Epsilon]},{1,-1}}],
  RotateText["Rule 12 \[DownArrow]", 1.15, ymax/2],

  Thick, Yellow, Line[{{xmin+\[Epsilon],1},{-1-2\[Epsilon],1}}],
  ShowText["Rule 13 \[LowerRightArrow]", (xmin-1)/2, 1.13],

  Thick, Lighter[Magenta,0.5], Line[{{-1,1},{xmax-\[Epsilon],1}}],
  ShowText["Rule 14 \[DownArrow]", (xmax+1)/2, 1.13],

  LabeledDisk["a",0,-1],
  LabeledDisk["b",0,1/2],
  LabeledDisk["b",0,-1/2],

  LabeledDisk["c",1,0],
  LabeledDisk["d",-1,0],
  LabeledDisk["e",-1/2,0],
  LabeledDisk["f",-3/2,0],
  LabeledDisk["f",1/2,0],

  LabeledDisk["g",-1,-1],

  LabeledDisk["h",-1,1/2],
  LabeledDisk["h",-1,-1/2],
  LabeledDisk["i",1/2,-1],
  LabeledDisk["i",-1/2,-1],

  LabeledDisk["j",-1/2,-1/2],
  LabeledDisk["k",1/2,-1/2],
  LabeledDisk["k",-1/2,1/2],

  LabeledDisk["l",-1/2,-3/2],
  LabeledDisk["m",-3/2,-1/2],
  LabeledDisk["n",1/2,-3/2],
  LabeledDisk["o",-3/2,1/2]
},
ImageSize->imagesize,
Axes->True,AxesLabel->{n,j k m}
]


(* ::Subsection::Closed:: *)
(*Domain Map of Integration Rules for (sin(x)^j)^m (A+B sin(x)^k+C sin(x)^(2k)) (a+b sin(x)^k)^n when a^2!=b^2*)
(**)


Graphics[{
  LabeledRectangle["Rule 1 \[LowerRightArrow]", Red, {xmin, 0}, {-1, ymax}, (ymax+1)/2],

  LabeledRectangle["Rule 2 \[DownArrow]", Lighter[Blue,0.5], {-1, 0}, {0, ymax}, (ymax+1)/2],
  Thick, Lighter[Blue,0.5], Line[{{-1, ymax-\[Epsilon]},{-1, 0}}],
  DashedLine[{-1+\[Epsilon], ymax-\[Epsilon]}, {-1+\[Epsilon], 0}],

  LabeledRectangle["Rule 3 \[LeftArrow]", Green, {0, -1}, {xmax, ymax}, (ymax+1)/2],
  DashedLine[{0+\[Epsilon], -1+\[Epsilon]}, {xmax-\[Epsilon], -1+\[Epsilon]}],

  LabeledRectangle["Rule 4 \[UpperLeftArrow]", Pink, {0,ymin},{xmax,-1}],

  LabeledRectangle["Rule 5 \[UpArrow]", Magenta, {-1, ymin}, {0, -1}, -2.5],
  Thick, Magenta, Line[{{-1,-1-\[Epsilon]},{-1,ymin+\[Epsilon]}}],
  DashedLine[{-1+\[Epsilon], -1-2\[Epsilon]}, {-1+\[Epsilon], ymin+\[Epsilon]}],

  LabeledRectangle["Rule 6 \[RightArrow]", Orange, {xmin, ymin}, {-1, 0}, -2.5],
  Thick, White, Line[{{-1,0-\[Epsilon]},{-1,-1}}],

  Thick, Yellow, Line[{{0,ymin+\[Epsilon]},{0,-1-\[Epsilon]}}],
  RotateText["Rule 7\[LeftArrow]\[Diamond]", 0.15, -2.5],

  Thick, Brown, Line[{{0,-1},{0,ymax-\[Epsilon]}}],
  RotateText["Rule 8\[Diamond]", 0.15, 2],

  Thick, Lighter[Green, 0.3], Line[{{xmin+\[Epsilon],0},{-1,0}}],
  ShowText["Rule 9 \[RightArrow]\[Diamond]", (xmin-1)/2, 0.13],

  Thick, Blue, Line[{{-1,0},{xmax-\[Epsilon],0}}],
  ShowText["Rule 10\[Diamond]", xmax/2, 0.13],

  LabeledDisk["a",-1,0],
  LabeledDisk["b",-1,-1],
  LabeledDisk["c",-1,-1/2],
  LabeledDisk["d",-1/2,-1],
  LabeledDisk["e",-1/2,-1/2],
  LabeledDisk["f",-1/2,-3/2],
  LabeledDisk["g",-3/2,-1/2]
},
ImageSize->imagesize,
Axes->True,AxesLabel->{n,j k m}
]


(* ::Subsection::Closed:: *)
(*Domain Map of Integration Rules for (sin(x)^j)^m (a+b sin(x)^k)^n when a^2=b^2*)


Graphics[{
  LabeledRectangle["Rule 1a \[LowerRightArrow]\[DownArrow]\[Diamond]", Red, {xmin, 1}, {-1, ymax}, (ymax+2)/2],
  DashedLine[{-1-\[Epsilon], ymax-\[Epsilon]}, {-1-\[Epsilon], 1+\[Epsilon]}],

  LabeledRectangle["Rule 1b \[LowerRightArrow]\[Diamond]", Lighter[Red,0.5], {xmin, 0}, {-1, 1}, {(xmin-2.5)/2, 0.5}],
  DashedLine[{-1-\[Epsilon], 1}, {-1-\[Epsilon], 0}],

  LabeledRectangle["Rule 2\[DownArrow]\[DownArrow]\[Diamond]", Lighter[Blue,0.7], {-1, 1}, {0, ymax}, (ymax+2)/2],
  Thick, White, Line[{{0,1},{1/2,3/2}}],

  LabeledRectangle["Rule 3a \[LeftArrow]\[LeftArrow]\[Diamond]", Lighter[Green,0.2], {1, -1}, {xmax, ymax}, {(xmax+2)/2, (ymax+2)/2}],
  DashedLine[{1+\[Epsilon], -1+\[Epsilon]}, {xmax-\[Epsilon], -1+\[Epsilon]}],

  LabeledRectangle["Rule 3b
\[LowerLeftArrow]  \[Diamond]", Lighter[Green,0.7], {0, 0}, {1, ymax}, (ymax+2)/2],

  LabeledRectangle["Rule 4a \[UpperLeftArrow]\[LeftArrow]\[Diamond]", Pink, {1, -1}, {xmax, ymin}, {(xmax+2)/2, (ymin-1)/2}],

  LabeledRectangle["Rule 4b  
\[UpperLeftArrow]  \[Diamond]", Lighter[Pink,0.6], {0, ymin}, {1, -1}, -2.1],

  LabeledRectangle["Rule 5 \[UpArrow]\[Diamond]", Magenta, {-1, ymin}, {0, -1}, (ymin-1)/2],

  LabeledRectangle["Rule 6 \[RightArrow]\[Diamond]", Orange, {xmin, ymin}, {-1, 0}, ymin/2],
  DashedLine[{-1-\[Epsilon], 0}, {-1-\[Epsilon], ymin+\[Epsilon]}],

  Thick, Green, Line[{{xmin+\[Epsilon], 0}, {-1, 0}}],
  ShowText["Rule 7 \[RightArrow]", (xmin-2)/2, 0.13],

  Thick, Red, Line[{{1, 0}, {xmax-\[Epsilon], 0}}],
  ShowText["Rule 8 \[LeftArrow]", (xmax+2)/2, 0.13],

  Thick, Green, Line[{{1/2, -1-\[Epsilon]}, {1/2, ymin+\[Epsilon]}}],
  RotateText["Rule 9 \[LeftArrow]", 0.65, -3.4],

  Thick, Orange, Line[{{1/2, ymax-\[Epsilon]}, {1/2, 0+\[Epsilon]}}],
  RotateText["Rule  10\[RightArrow]", 0.65, 2],

  Thick, Yellow, Line[{{0, -1}, {-ymin-1, ymin}}],
  RotateText["Rule 11 \[LeftArrow]", 2.6, -3.4, -45 Degree],

  Thick, Blue, Line[{{xmin+\[Epsilon], -xmin-\[Epsilon]-1}, {-1-\[Epsilon], \[Epsilon]}}],
  RotateText["Rule 12 \[RightArrow]", -2.3, 1.5, -45 Degree],

  Thick, Yellow, Line[{{-1, -1}, {-ymin-2, ymin}}],
  RotateText["Rule 13 \[UpperLeftArrow]", 1.6, -3.4, -45 Degree],

  Thick, Blue, Line[{{xmin+\[Epsilon], -xmin-\[Epsilon]-2}, {-1-\[Epsilon], -1+\[Epsilon]}}],
  RotateText["Rule 14 \[UpperRightArrow]", -3.35, 1.55, -45 Degree],

  Thick, Black, Line[{{0,ymax-\[Epsilon]},{0,ymin+\[Epsilon]}}],
  Thick, Black, Line[{{1,ymax-\[Epsilon]},{1,ymin+\[Epsilon]}}],
  Thick, Black, Line[{{2,ymax-\[Epsilon]},{2,ymin+\[Epsilon]}}],

  Thick, Black, Line[{{xmin+\[Epsilon],1},{xmax-\[Epsilon],1}}],
  Thick, Black, Line[{{xmin+\[Epsilon],2},{xmax-\[Epsilon],2}}],

  TerminalRule["a",-1,0],
  TerminalRule["b",1/2,0],
  TerminalRule["c",-1/2,0],

  TerminalRule["d",1/2,-1],
  LabeledDisk["e",-1/2,-1],
  TerminalRule["f",-1/2,-1/2],
  TerminalRule["g",1/2,-1/2],
  LabeledDisk["h",-1/2,1/2],

  LabeledDisk["i",-2,1/2],
  LabeledDisk["i",-2,3/2]
},
ImageSize->imagesize,
Axes->True,AxesLabel->{n,j k m}
]


(* ::Subsection::Closed:: *)
(*Domain Map of Integration Rules for (sin(x)^j)^m (A+B sin(x)^k) (a+b sin(x)^k)^n when a^2=b^2*)


Graphics[{
  LabeledRectangle["Rule 1 \[LowerRightArrow]", Red, {xmin, 0}, {-1, ymax}, 2.5],
  DashedLine[-1-\[Epsilon],ymax-\[Epsilon],-1-\[Epsilon],0+\[Epsilon]],

  LabeledRectangle["Rule 2 \[DownArrow]", Lighter[Blue,0.7], {-1, 0}, {0, ymax}, 2.5],

  LabeledRectangle["Rule 3 \[LeftArrow]", Green, {0, -1-\[Epsilon]}, {xmax, ymax}, 2.5],
  DashedLine[{0+\[Epsilon], -1+\[Epsilon]}, {xmax-\[Epsilon], -1+\[Epsilon]}],

  LabeledRectangle["Rule 4 \[UpperLeftArrow]", Pink, {0, -1}, {xmax, ymin}],

  LabeledRectangle["Rule 5 \[UpArrow]", Magenta, {-1, ymin}, {0, -1-\[Epsilon]}, -2.5],

  LabeledRectangle["Rule 6 \[RightArrow]", Orange, {xmin, ymin}, {-1, 0}, -2.5],
  DashedLine[-1-\[Epsilon], 0, -1-\[Epsilon], ymin+\[Epsilon]],

  Thick, Green, Line[{{xmin+\[Epsilon],0},{-1,0}}],
  ShowText["Rule 7 \[RightArrow]\[Diamond]", (-2+xmin)/2, 0.13],

  Thick, Red, Line[{{-1+\[Epsilon],0},{xmax-\[Epsilon],0}}],
  ShowText["Rule 8\[Diamond]", xmax/2, 0.13],

  Thick, Yellow, Line[{{-1,-1},{-ymin-2,ymin}}],
  RotateText["Rule 11 \[UpperLeftArrow]\[Diamond]", 1.6, -3.39, -45 Degree],

  Thick, Blue, Line[{{xmin,-xmin-2},{-1-\[Epsilon],-1}}],
  RotateText["Rule 12 \[UpperRightArrow]\[Diamond]", -2.5, 0.7, -45 Degree],

  Thick, Green, Line[{{1/2,-1-\[Epsilon]},{1/2,ymin+\[Epsilon]}}],
  RotateText[" Rule 9 \[LeftArrow]\[Diamond]", 0.65, -3.4],

  Thick, Orange, Line[{{1/2,ymax-\[Epsilon]},{1/2,-1+\[Epsilon]}}],
  RotateText["Rule 10\[Diamond]", 0.65, 1.5],

  LabeledDisk["a",-1,0],
  LabeledDisk["b",-1/2,0],

  LabeledDisk["c",-1/2,-1],
  LabeledDisk["d",-1/2,-1/2],

  BlackDisk[-1, 1]
},
ImageSize->imagesize,
Axes->True,AxesLabel->{n,j k m}
]


(* ::Subsection::Closed:: *)
(*Domain Map of Integration Rules for (sin(x)^j)^m (A+B sin(x)^k+C sin(x)^(2k)) (a+b sin(x)^k)^n when a^2=b^2*)


Graphics[{
  LabeledRectangle["Rule 1 \[RightArrow]\[Diamond]", Red, {xmin, ymin}, {-1, ymax}, ymax/2],
  DashedLine[{-1-\[Epsilon], ymax-\[Epsilon]}, {-1-\[Epsilon], ymin+\[Epsilon]}],

  LabeledRectangle["Rule 2 \[UpArrow]\[Diamond]", Magenta, {-1, ymin}, {xmax, -1}, {xmax/2, -5/2}],

  LabeledRectangle["Rule 3\[Diamond]", Green, {-1, -1}, {xmax, ymax}, {xmax/2, ymax/2}],
  DashedLine[{-1-\[Epsilon], -1+\[Epsilon]}, {xmax-\[Epsilon], -1+\[Epsilon]}],

  Thick, Lighter[Blue,0.5], Line[{{xmin+\[Epsilon], 0}, {-1-\[Epsilon], 0}}],
  ShowText["Rule 4 \[RightArrow]\[Diamond]", (xmin-1)/2, 0.13],

  Thick, Black, Line[{{-1, 0}, {xmax-\[Epsilon],0}}]
},
ImageSize->imagesize,
Axes->True,AxesLabel->{n,j k m}
]
