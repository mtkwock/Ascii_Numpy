import cv
from os import system as sys
from ticktock import *

tick()
im = cv.LoadImageM("photos/drag.jpg")
dst = cv.CreateImage(cv.GetSize(im), cv.IPL_DEPTH_16S, 3)
gray = cv.CreateImage(cv.GetSize(im), cv.IPL_DEPTH_8U, 1)
cv.Laplace(im, dst)


cv.CvtColor(dst, gray, cv.CV_RGB2GRAY)
cv.SaveImage("foo-laplace.png", gray)
sys("xdg-open foo-laplace.png")
tock()