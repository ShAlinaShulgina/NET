Клиент-сервер с TCP и UDP.
<br/>
##  Запуск.
```
./client port
./server port
```
##  Алгоритм.
* Клиент ищет сервер по бродкаст рассылке, отправляя принятое из stdin сообщение. 
* Сервер принимает бродкаст сообщение и отвечает, что готов принять соединение.
* Клиент устанавливает с серверос TCP соединение, читает строку из stdin и отправляет ее серверу.
* Сервер принимает сообщение, проверяет его на палиндром и отвечает 1 или 0.
