// connect ao FTP, chamando connect socket

// connect socket:

	// abrir socket para connect ao FTP
	// login no FTP
	// esperar resposta (replyCode), checkar e atuar
	// se der erro, terminar socket, terminar programa
	
	// entrar em modo passivo, calcular os últimos 2 bytes (penultimo * 256 + ultimo)
	// os primeiros 4 bytes serão o ip do segundo socket, que servirá para fazer download
	// abrir socket e guardar
	// pedir ficheiro ao FTP atraves do primeiro socket
	// usar o segundo socket e ler o ficheiro atraves de packets (ler, escrever, ler, escrever)
	// fechar os 2 sockets (close do fd). antes, mandar comando quit para primeiro
	// terminar programa