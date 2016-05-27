# Systemy wbudowane projekt 
Projekt stacji pogodowej z wykorzystaniem wyświetlacza dotykowego opartej o Cortex-M4F.

## Części:
- płytka ewaluacyjna [STM32F429I](http://www2.st.com/content/ccc/resource/technical/document/data_brief/ff/c1/b6/02/c3/b4/49/cb/DM00094498.pdf/files/DM00094498.pdf/jcr:content/translations/en.DM00094498.pdf)
- czujnik wewnętrzny temperatury i wilgotności [DHT-11](http://www.micropik.com/PDF/dht11.pdf) 
- termometr cyfrowy zewnętrzny [DS18B20](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf) (1-wire)
- moduł czujnika poziomu CO_2 w pomieszczeniu oparty na [MG811](http://www.dfrobot.com/image/data/SEN0159/CO2b%20MG811%20datasheet.pdf) (analog)
- barometr cyfrowy [LPS331](http://www2.st.com/content/ccc/resource/technical/document/datasheet/40/c2/46/1e/99/22/4c/60/DM00036196.pdf/files/DM00036196.pdf/jcr:content/translations/en.DM00036196.pdf) (SPI/I2C)

Wstępny schemat części elektronicznej (dzielniki napięcia prawdopodobnie zostaną poprzedzone wtórnikami)
[link](https://github.com/KrzysztofStachanczyk/SysWbud1/blob/master/electronic_schem.pdf)

W ramach projektu zostanie wykonana również shield z w.w. sensorami.

#Dokumentacja
## Opis czujników i ich interfaców:
Do wykonania projektu wykorzysztano różne czujniki cyfrowe i analogowe. Interfacy części z nich musiały zostać programowo zaemulowane ze względu na brak ich sprzętowego wsparcia. 
### DHT-11:
Jest to czujnik temperatury i wilgotności posiadający własny cyfrowy interface komunikacyjny wykorzystujący jedną szynę danych i wejścia typu open-drain. 

Parametry czujnika to:
- zakres pomiarowy temperatury 0-50 stopni, wilgotności 20-90%
- rozdzielczość temperatury i wilgotności 8 bitów
- niepewność temperatury +/- 1 stopień , wilgotności +/- 4%

Szyna danych czujnika podpięta jest do pinu PG5.

Aby dokonać pomiaru należy wywołać funkcję:
```
DHT11_read();
```
Która sprawdza czy pin jest skonfigurowany i wywołuje inne funkcje związane z inicjalizacją komunikacji, odebraniem i dekodowaniem danych oraz wyrywaniem błędów transmisji. W celu uniknięcia zawieszenia się urządzenia wprowadziliśmy wykrywanie time-outu (braku wystepowania zmian sygnałów na linii danych) wynoszącego 500uS (w czasie normalnej komunikacji linia danych nie powinna pozostawać w tym samym stanie przez dłużej niż 80uS). Ponadto po odczytaniu danych sprawdzamy sumę kontrolną i w przypadku wystąpienia niezgodności ustawiamy odpowiedni kod błędu. Dane dotyczące w.w. kodu oraz wyników pomiarów zwracane są jako struktura  DHT11Result:
```
enum DHT11_ERRORS{
	DHT11_OK,
	DHT11_CRC_ERROR,
	DHT11_NO_RESPONSE,
	DHT11_TIME_OUT,
	DHT11_DECODE_ERROR
};

typedef struct DHT11Result{
	float temperature;
	float humidity;
	int ERROR_CODE;
}DHT11Result;
```

W celu wprowadzenia wiarygodnego pomiaru czasu występowania stanów skonfigurowaliśmy timer TIM2 tak aby zliczał on w górę w tempie 1 inkrementacji na mikrosekundę. 

#### Opis protokołu:
- mater wymusza stan niski na lini danych (PULL-UP) na czas 20ms
- w ciągu 20-40 us slave powinien wymusić stan niski  i podtrzymać go przez 80uS ( jeżeli tego nie zrobi zwracany jest kod błędu DHT11_NO_RESPONSE ) 
- po 80uS stanu wysokiego rozpoczyna się proces nadawania przez slave kolejnych bitów (najpierw wysyłane są bity MSB) następujących pól 8bitowych:
  1. cyfra jedności RH
  2. cyfra dziesiątek RH
  3. cyfra jedności temperatury
  4. cyfra dziesiątek temperatury
  5. suma kontrolna
  
  Każdy bit poprzedzony jest 50uS wystąpieniem stanu niskiego. Następnie dla "1" linia danych wprowadzana jest w stan wysoki na czas 70uS (przyjmujemy wartości z przedziału 50-90 uS) w przypadku "0" 26-28 uS (10-40 uS).
  
  Jeżeli któryś z wykrytych czasów nie mieści się w zadanych przedziałach zwracany jest kod błędu DHT11_DECODE_ERROR.

#### Suma kontrolna:
Wartość sumy kontrolnej wyznaczana jest według wzoru:
```
uint8_t checksum=   cyfra jedności RH 
                  + cyfra dziesiątek RH 
                  + cyfra jedności temperatury 
                  + cyfra dziesiątek temperatury
```

Jeżeli jest niezgodna to zwracany kod błędu DHT11_CRC_ERROR.

### DS18B20
Cyfrowy czujnik temperatury o zakresie pomiarowym -55 °C do 125 °C i dokładności +/- 0,5 stopnia. Rozdzielczości pomiarów 9-12 bitów. Wykorzystujący protokół one-wire (emulowany programowo).

Przed odczytaniem wartości z czujnika temperatury należy rozpocząć konwersję trwającą maksymalnie 720ms:
```
ds18b20_beginConversion();
```

Następnie po odczekaniu w.w czasu należy wywołać funkcję:
```
ds18b20_read();
```

Która zwraca strukture zawierajaca wartość temperatury i kominikat o błędzie:
```
enum ds18b20_ERRORS{
	ds18b20_OK,
	ds18b20_NO_RESPONSE,
	ds18b20_STRANGE_VALUE
};

typedef struct ds18b20Result{
	float temperature;
	int ERROR_CODE;
}ds18b20Result;
```

Ze względu na ominięcie adresowania można podpiąć tylko jeden czujnik/układ do linii danych (PG4). Temperatura jest wyznaczana z maksymalną (12 bitową) dokładnością.

Do realizacji opóźnień na poziomie mikrosekund wykorzystano timer TIM2 (podobnie jak w przypadku DHT11).

#### Proces rozpoczynania jednorazowej konwercji:
- resetowanie układu
- odczekanie na potwierdzenie obecności czujnika (jeśli nie występuje to zwracany kod błędu ds18b20_NO_RESPONSE)
- wysłanie wartości DS18B20_CMD_SKIPROM informującej o tym, że komunikat skierowany jest do dowolnego urządzenia slave na linii danych - ominięcie adresowania
- wysłanie komendy DS18B20_CMD_CONVERTTEMP wyzwalającej konwersję

#### Odczyt danych z czujnika temperatury:
- resetowanie układu
- odczekanie na potwierdzenie obecności czujnika (jeśli nie występuje to zwracany kod błędu ds18b20_NO_RESPONSE)
- wysłanie wartości DS18B20_CMD_SKIPROM informującej o tym, że komunikat skierowany jest do dowolnego urządzenia slave na linii danych - ominięcie adresowania
- wysłanie komendy DS18B20_CMD_RSCRATCHPAD odpowiedzialnej za dostęp do rejestrów zawierających informacje o stanie urządzenia i zmierzonych wartościach
- odczytanie wartości dwóch pierszych rejestrów:
  - Temperature LSB 
  - Temperature MSB

#### Wyznaczanie wartości liczbowej temperatury:
Na podstawie dokumentacji, cały rejestr LSB oraz 3 najmłodsze bity MSB niosą informację o wartości bezwzględnej tempretury (skok co 0,0625 stopnia) natomiast bit 4 jest bitem znaku. 
```
// combine data
  result.temperature=(((0x07 &temperature_h) << 8) + temperature_l ) * 0.0625;

// check sign
  if(temperature_h & (1<<4)){
    result.temperature*=-1;
  }
```







