
# VLOCK 

Arduino project for E.E.T.N°2043 "San José" 2022


## How to use it?
This project has 3 different versions
- #### [Bluetooth and WiFi mode (not recommended without SSL certification)](https://github.com/avryux/Vlock/blob/main/ESP32/ESP32_WiFi_BL.ino)
- #### [WiFi mode](https://github.com/avryux/Vlock/blob/main/ESP32/ESP_WiFi.ino)
```c++
  #define WIFI_SSID ""
  #define WIFI_PASSWORD ""
  #define API_KEY ""
  #define DATABASE_URL ""
```

| Vartiable | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `WIFI_SSID` | `string` | **Required**. Your WiFi SSID |
| `WIFI_PASSWORD` | `string` | **Required**. Your WiFi Password |
| `API_KEY` | `string` | **Required**. Your Firebase API key |
| `DATABASE_URL` | `string` | **Required**. Your Firebase RTDB URL |

```c++
  const long  gmtOffset_sec = -14400;
```
| Vartiable | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `gmtOffset_sec` | `long` | **Required**. offset in seconds between your time zone and GMT |

- #### [Bluetooth Mode](https://github.com/avryux/Vlock/blob/main/ESP32/ESP32_BL.ino)

## Authors

- [Verdondoni, Máximo](https://ar.linkedin.com/in/máximo-verdondoni-846821214)
- Leoni, Juan Martín


## 🛠 Skills
- For Web App development: HTML, CSS and Javascript
- For Arduino and ESP32 development: C++



## Feedback

If you have any feedback, please reach out to us at mverdondoni04@sanjoserosario.com.ar or jmleoni04@sanjoserosario.com.ar

