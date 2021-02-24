# qcustomplotdemo

![alt tag](screenshot.png)

The "interactive" example from Qcustomplot but built system is `cmake`
and an efficient realtime plot has been added.

Plots two random waves and one animated wave by shifting the
samples by one step and then replacing the 1st sample by a new one.

## Required packages

Install the QT5 development packages:

```
    apt-get install qtdeclarative5-dev-tools
```

## Build it

```
    cmake .
    make
```

## Run it

```
./qcustomplotdemo
```

## Credit

QCustomPlot:

https://www.qcustomplot.com/
