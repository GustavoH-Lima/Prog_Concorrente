//introduz canais na aplicação Go
package main

import (
	"fmt"
)

func tarefa(str chan string) {
	//escreve no canal
	var msg = ""
	msg = <-str
	fmt.Println("Goroutine:", msg)
	str <- "Olá, Main, bom dia,tudo bem?"

	msg = <-str
	fmt.Println("Goroutine:", msg)
	str <- "Certo, entendido."
	fmt.Printf("Goroutine: finalizando\n")
	str <- ""
}

func main() {
	var msg = "" //string vazia
	
	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string)
	
	//cria um goroutine que executará a função 'tarefa'
	go tarefa(str)

	//lê do canal

	str <- "Olá, Goroutine, bom dia!"
	msg = <-str
	fmt.Println("Main:", msg)

	str <- "Tudo bem! Vou terminar tá?"
	msg = <-str
	fmt.Println("Main:", msg)

	msg = <-str
	fmt.Println("Main: finalizando")
}
