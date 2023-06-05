# SigmaGeek - Largest Pallindrome Prime in pi digits
 My submission on the challenge. I found the largest.

There were 3 phases of this challenge

1) Find first 9-digit pallindrome prime in pi digits
318272813

2) Find first 21-digit pallindrome prime in pi digits
size 21 151978145606541879151
//near digit position of 140672630233

3) Find largest pallindrome prime
//size 23 38898292453535429289883
//near digit position of 2143728293286
//size 25 9609457639843489367549069
//near digit position of 33135773126758

To finish all three phases, i processed all 1000 ycd files from this source (https://pi.delivery/). The dataset contains the first 100 trillion digits of pi decimal expansion. To achive this, i automatized the ycd splitting process by modifying y-cruncher open source code. After the splitting, i used a c program to read the split and find the pallindromes. I also printed the total elapsed time to detect whether the ycd got corrupted during the download from curl command. If the total elapsed time is 0, the split size is 0, then it meant the ycd got corrupted at some point, and therefore, didn't achive full original size after download, so I knew I had to download and process that part again.

