# Examples for Arduino Workshop

1. [Hello World](https://github.com/dhavalgujar/iot-workshop/blob/step-1/code.ino)
2. [Blink](https://github.com/dhavalgujar/iot-workshop/blob/step-2/code.ino) [[`What was added in this step?`](https://github.com/dhavalgujar/iot-workshop/compare/step-1...step-2)]
3. [Push Button](https://github.com/dhavalgujar/iot-workshop/blob/step-3/code.ino)
4. [Push Button with LED](https://github.com/dhavalgujar/iot-workshop/blob/step-4/code.ino) [[`What was added in this step?`](https://github.com/dhavalgujar/iot-workshop/compare/step-3...step-4)]
5. [Rainmaker Basic Switch](https://github.com/dhavalgujar/iot-workshop/blob/step-5/code.ino)
6. [Rainmaker LED with RGB controls](https://github.com/dhavalgujar/iot-workshop/blob/step-6/code.ino) [[`What was added in this step?`](https://github.com/dhavalgujar/iot-workshop/compare/step-5...step-6)]
7. [Rainmaker LED with HSV controls (VA supported)](https://github.com/dhavalgujar/iot-workshop/blob/step-7/code.ino) [`[What was added in this step?`](https://github.com/dhavalgujar/iot-workshop/compare/step-6...step-7)]

## Get comfortable with abstractions

![Get comfortable with abstractions](computer_programming_101.png)

## Not the right way to get comfortable with abstractions ;)

![Quick answer](quick_answer.png)

## Helper functions:
<details>
   <summary>HSV to RGB</summary>

   ```c
   static void convertHSVtoRGB(int h, int s, int v, int *r, int *g, int *b)
   {
       h %= 360; // h -> [0,360]
       uint32_t rgb_max = v * 2.55f;
       uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;
       uint32_t i = h / 60;
       uint32_t diff = h % 60;
       // RGB adjustment amount by hue
       uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;
       switch (i) {
       case 0:
           *r = rgb_max;
           *g = rgb_min + rgb_adj;
           *b = rgb_min;
           break;
       case 1:
           *r = rgb_max - rgb_adj;
           *g = rgb_max;
           *b = rgb_min;
           break;
       case 2:
           *r = rgb_min;
           *g = rgb_max;
           *b = rgb_min + rgb_adj;
           break;
       case 3:
           *r = rgb_min;
           *g = rgb_max - rgb_adj;
           *b = rgb_max;
           break;
       case 4:
           *r = rgb_min + rgb_adj;
           *g = rgb_min;
           *b = rgb_max;
           break;
       default:
           *r = rgb_max;
           *g = rgb_min;
           *b = rgb_max - rgb_adj;
           break;
       }
   }
   ```
</details>

*Comic courtesy of [Abtruse Goose](http://abstrusegoose.com/98)*
