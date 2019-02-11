# ThinGPS Library Arduino

Uma biblioteca simples para tratamento de dados NMEA utilizando GPS. Permite a captura de dados, como: latitude, longitude, hora, data e velocidade.

## Protocolo NMEA 

O padrão NMEA 0183 define, dentre outros, características elétricas do sinal, protocolo da transmissão de dados, sincronismo e formatos específicos de sentenças de transmissão. 
Mais informações sobre o padrão NMEA 0183 pode ser obtido em www.nmea.org

O protocolo NMEA 0183 consiste em vários tipos de sentenças, sendo que cada uma contém um nome no início que permite sua identificação. Alguns dados são redundantes em diferentes tipos de sentenças. Abaixo estão listadas algumas e quais a biblioteca utiliza para obter os dados:

- GPGGA – Global Positioning System Fix Data (Implementado)
- GPRMC – Recommended Minimum Specific GPS/Transit Data (Implementado)
- GPVTG – Course Over Ground and Ground Speed
- GPGSV – GPS Satellite in View


#### Testado com:
- Arduino Mega 2560
- [Arduino GPS Shield Itead](https://www.itead.cc/wiki/Arduino_GPS_shield)


## Capacidade da Biblioteca 

|Retorno| Métodos Públicos | Descrição|
|--------|---------------------------|------|
| void | begin (unsigned long baud) | Define a taxa de transmissão serial do GPS |
| char | read () | Leitura dos dados nmea |
| void | update () | Atualiza os dados |
| boolean | is_ready () | Retorna se os dados estão válidos para leitura |
| boolean | is_validnmea (char* nmea) | Retorna se string nmea é válida |
| boolean | have_newnmea () | Retorna se existe novos dados  |
| char | get_newnmea () | Retorna a string nmea |
| void | send_command (const char *cmd) | Envia um comando para o GPS |

## Licença 
Para mais informações sobre a licença consulte o arquivo [LICENSE.txt](https://github.com/luanfssilva/ThinGPS-Library-Arduino/blob/master/LICENSE.txt)