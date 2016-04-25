# Systemy wbudowane projekt
Projekt stacji pogodowej z wykorzystaniem wyświetlacza dotykowego opartej o Cortex-M4F.

## Części:
- płytka ewaluacyjna [STM32F429I](http://www2.st.com/content/ccc/resource/technical/document/data_brief/ff/c1/b6/02/c3/b4/49/cb/DM00094498.pdf/files/DM00094498.pdf/jcr:content/translations/en.DM00094498.pdf)
- czujnik wewnętrzny temperatury i wilgotności [DHT-11](http://www.micropik.com/PDF/dht11.pdf) 
- termometr cyfrowy zewnętrzny [DS18B20](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf) (1-wire)
- moduł czujnika poziomu CO_2 w pomieszczeniu oparty na [MG811](http://www.dfrobot.com/image/data/SEN0159/CO2b%20MG811%20datasheet.pdf) (analog)
- barometr cyfrowy [LPS331](http://www2.st.com/content/ccc/resource/technical/document/datasheet/40/c2/46/1e/99/22/4c/60/DM00036196.pdf/files/DM00036196.pdf/jcr:content/translations/en.DM00036196.pdf) (SPI/I2C)
- demodulator podczerwieni [TSOP31230](http://www.vishay.com/docs/82492/tsop312.pdf) (przerwanie EXT_x i timer) 
- czujnik poziomu pyłów w powietrzu [GP2Y1010AU0F](https://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf) (analog)
- pilot do tunera telewizyjnego Hama

Wstępny schemat części elektronicznej (dzielniki napięcia prawdopodobnie zostaną poprzedzone wtórnikami)
[link](https://github.com/KrzysztofStachanczyk/SysWbud1/blob/master/electronic_schem.pdf)

W ramach projektu zostanie wykonana również shield z w.w. sensorami.

