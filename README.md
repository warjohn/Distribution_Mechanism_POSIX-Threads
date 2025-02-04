# Программа для распределения пакетов

## Описание

Данная программа предназначена для обработки данных из файла в формате `.pcap`, содержащего пакеты UDP или TCP. Программа распределяет пакеты по трем обработчикам в зависимости от критериев в поле назначения IP и порта.

## Как работает программа

Программа выполняет следующие шаги:

1. Принимает путь к файлу с данными в формате `.pcap` как обязательную опцию командной строки.
2. Программа поочередно извлекает пакеты из файла и передает их в распределительный механизм.
3. В зависимости от содержимого пакета, он направляется к одному из трех обработчиков:
   - **Обработчик 1**: Если **dst IP** находится в диапазоне `11.0.0.3`-`11.0.0.200`.
   - **Обработчик 2**: Если **dst IP** находится в диапазоне `12.0.0.3`-`12.0.0.200` и **dst port** равен `8080`.
   - **Обработчик 3**: Все остальные пакеты.

## Логика работы обработчиков

Каждый обработчик выполняет специфические действия с пакетами согласно следующим правилам:

- **Обработчик 1**:
  - Если **dst port** равен `7070`, пакет не записывается в файл, а на экране выводится уведомление:
    ```
    Обработчик 1: пакет под номером {номер пакета} игнорируется
    ```
- **Обработчик 2**:
  - Если в данных L4 пакета встречается символ `'x'`, все символы после него обрезаются, и пакет записывается в файл с символом `'x'` в конце.
- **Обработчик 3**:
  - Если пакет TCP, программа ждет 2 секунды, затем проверяет текущее системное время в секундах. Если оно кратно 2, пакет записывается в файл.
  - Если пакет UDP, и **src port** совпадает с **dst port**, пакет записывается в файл, и выводится уведомление:
    ```
    Обработчик 3: Найдено совпадение port = {номер порта}
    ```

## Запуск программы

Для запуска программы необходимо передать путь к файлу `.pcap` с пакетами:

```bash
./Main /path/to/your/file.pcap
```

## Структура выходных файлов

Каждый обработчик создает свой собственный выходной файл в формате .pcap:

1. result_1.pcap — файл для обработчика 1.
2. result_2.pcap — файл для обработчика 2.
3. result_3.pcap — файл для обработчика 3.

## Используемые технологии

1. Программа использует POSIX Threads для многозадачности (параллельная обработка пакетов).
2. Для синхронизации потоков могут использоваться очереди или другие механизмы синхронизации, такие как буферы.