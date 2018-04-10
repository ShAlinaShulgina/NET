Клиент должен обнаружить сервер через **broadcast** рассылку.
<br/>
##  Протокол.
```
struct data
{
	unsigned int id;
	int flag;
	char str[128];
};
```
##  Запуск.
```
make
./client port
./server port
```
