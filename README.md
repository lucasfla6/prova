# prova
O projeto e composto pelos seguintes arquivos fonte:

p1.c: Programa de processamento de dados brutos.

p2.c: Programa de consulta de leituras de sensores.

p3.c:Programa de geracao de dados brutos aleatorios.

Compilacao
Para compilar os programas, e necessario ter um compilador C (como o GCC) instalado. Navegue ate o diretorio onde os arquivos .c estao salvos e execute os seguintes comandos:

gcc p1.c -o p1

gcc p2.c -o p2

gcc p3.c -o p3

Passo 3: Execute os programas

A ordem de execução é importante, pois p1 e p2 dependem dos arquivos gerados por p3.

Gerar dados brutos com p3:

Primeiro, execute o p3 para criar o arquivo dados_brutos_gerados.txt. Você precisa especificar um intervalo de datas/horas e os IDs/tipos dos sensores.

Exemplo de uso:

./p3 1 1 2024 0 0 0 31 12 2024 23 59 59 SENSOR_TEMP:CONJ_Q SENSOR_PRES:CONJ_Z SENSOR_TEXTO:TEXTO SENSOR_BIN:BINARIO

Processar dados e criar arquivos por sensor com p1:

Agora, use o p1 para ler dados_brutos_gerados.txt e organizar as leituras em arquivos separados para cada sensor (ex: SENSOR_TEMP.txt, SENSOR_PRES.txt, etc.).

Comando:

./p1 dados_brutos_gerados.txt

Consultar dados específicos com p2:
Finalmente, você pode usar o p2 para buscar a leitura mais próxima para um sensor e timestamp específicos.

Exemplo de uso:
./p2 SENSOR_TEMP 15 6 2024 10 30 0

