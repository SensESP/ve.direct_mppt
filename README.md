# ve.direct_mppt
Library for reading one victron mppt unit via the ve.direct hardware interface into SignalK/SensESP.

This library allows you to use Victron ve.direct interfaces with SignalK/SensESP, focused on MPPT devices. Victron ve.direct is a serial TTL interface that is available on Victron devices. Here's how to use it.

1. Follow the instructions in the SensESP [README](https://github.com/SignalK/SensESP#sensesp) to install Signal K
and SensESP.

2. Don't try to use this library until after you have successfully built and uploaded one of the SensESP examples
and seen the desired output in the Signal K Server.

3. Start a new Project in PlatformIO.

4. Follow the instructions in the SensESP README (and the Wiki if you need [detailed instructions](https://github.com/SignalK/SensESP/wiki/SensESP-Overview-and-Programming-Details#getting-a-good-platformioini-file)) for getting a good `platformio.ini` file for
your Project.

5. Make one modification to your shiny new `platformio.ini` - add this library to the `lib_deps` section, like this:
```
lib_deps =
   SignalK/SensESP
   https://github.com/SensESP/ve.direct_mppt
```

6. Replace your Project's `main.cpp` file with the [example file](https://github.com/SensESP/ve.direct_mppt/blob/main/examples/main.cpp) that's included with this library.

7. Modify your `main.cpp` to reflect the GPIO number of the pin that you have connected your ve.direct interface to. It is recommended to include galvanic isolation to connect the TxD of ve.direct interface to the GPIO pin.

8. Build, upload, and test your Project.

Once you have the example working properly, you can include this library in any SensESP Project just by following Step 5 above.


