# 2D-Texture Packing Algorithm

**Please read the following notes carefully before using this program**

(1)In the "Document" folder, the doc version of the report is combined with the pdf version, as well as all the results of the test set.

(2)In the "Code" folder, the source code of the project is included. If you are not sure how to use the program, here we give an example of the input:
*************************************************************
## usage example
```
(Please select testing algorithm runing situation or just input with result!
Key [1] : Testing mode!
Key [2] : Input mode!)
input:1
(Please input the number of boxes:)
input:100
(Please input the given width(<=10000):)
input:1000
(Please input box's max height and max width for random generating:)
input:30 50
(Please decide whether testing on-line for it runs so slow! 1 for yes while 0 for no!)
input:1
(Please input the number of test times:)
input:100
*************************************************************
output:
[1] Testing result for on-line algorithm:
[1.1] Rotation denied:
onLine-ordinary: 58  Average Testing Time: 2.154ms Appoximate-ratio: 1.48
onLine-faster: 58  Averate Testing Time: 0.759ms Appoximate-ratio: 1.50
[1.2] Rotation permited:
onLine-ordinary: 57  Average Testing Time: 1.974ms Appoximate-ratio: 1.47
onLine-faster: 57  Average Testing Time: 0.720ms Appoximate-ratio: 1.48
[2] Testing result for off-line algorithm:
[2.1] Rotation denied:
N-FFDH: 49  Average Testing Time: 0.084ms Appoximate-ratio: 1.27
FFDH: 55  Average Testing Time: 0.037ms Appoximate-ratio: 1.42
NFDH: 55  Average Testing Time: 0.024ms Appoximate-ratio: 1.42
BFDH: 55  Average Testing Time: 0.027ms Appoximate-ratio: 1.42
[2.2] Rotation permited:
N-FFDH: 48  Average Testing Time: 0.068ms Appoximate-ratio: 1.23
FFDH: 55  Average Testing Time: 0.027ms Appoximate-ratio: 1.41
NFDH: 55  Average Testing Time: 0.023ms Appoximate-ratio: 1.41
BFDH: 55  Average Testing Time: 0.025ms Appoximate-ratio: 1.41
```