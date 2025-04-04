# Сборка

- Соберите устройство согласно [схеме датчика](../LedPhotoDistributedSerial/sensor_scheme.pdf) и [cхеме светодиода](../LedPhotoDistributedSerial/light_scheme.pdf)
- Загрузите скрипты **sensor_part** и **light_part** в соответствующие устройства
- Проверьте порты, на которых работают устройства, и замените значения на нужные в pub.py и subscriber.py

# Использование

- Для обработки сообщений в реальном времени потребуется библиотека **threading**, установите её, если необходимо.
- Установите библоитеки **pyserial**, **paho-mqtt**, **numpy** и **hashlib**.
- Допустимо подключать устройства к 1 компьютеру. Для этого генератор уникальных id использует разные части mac адреса.

# Демонстрация работы (аудиторное задание)

- Увидеть работу устройства можно по [ссылке](https://drive.google.com/file/d/1Z14n2ValiGvADqUspWIhRGW7RSITMHfa/view?usp=sharing)

# Демонстрация работы

- Увидеть работу устройства можно по [ссылку](https://drive.google.com/file/d/1_z0QdCRm1spADhrUmjzm0G4as1VzRjJD/view?usp=sharing). Однако часть работы светодиода сложно продемонстрировать (например, что светодиод светится только на участках падения показаний).
- Получение unique_id без хардкода!!!