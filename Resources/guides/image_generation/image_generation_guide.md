# Image Generation Guide

1. Import to Adobe Photoshop
2. Crop the image to 800x480 pixels
3. Convert to grayscale
4. Convert to bitmap
5. Rotate to landscape (if image is in portrait)
6. Export or Save as gif
7. Install Image2Lcd program -> https://www.waveshare.com/wiki/File:Image2Lcd.7z
8. Import image into Image2Lcd
9. Set width to 800
10. Set height to 480
11. Uncheck include head data
12. Set output file as C array
13. Set scan mode as horizontal mode
14. Set BitsPixels as monochrome
15. Check reverse color
16. Save file
17. Check the file is in the correct format:
    -> const unsigned char terry[48000] = {0XFF, 0XFF, ...} 


