/* Extraído da documentação de Go */

package main
import (
	"fmt"
	"math"
    "os"
    "strconv"
)

// Função para verificar se um número é primo
func ehPrimo(n int64) int {
	if n <= 1 {
		return 0
	}
	if n == 2 {
		return 1
	}
	if n%2 == 0 {
		return 0
	}
	limite := int64(math.Sqrt(float64(n))) + 1
	for i := int64(3); i < limite; i += 2 {
		if n%i == 0 {
			return 0
		}
	}
	return 1
}

func threads(jobs chan int, resp chan int,id int){

    var total = 0
    for {
        num,continua := <- jobs
        if !continua{
            // fmt.Printf("Gorotina terminou\n")
            resp <- total
            return
        }
        // fmt.Printf("Rotina %d: Executando o número %d\n",id,num)
        total+=ehPrimo(int64(num))
    }
}

func main() {
    
    var total = 0
    respostas := make(chan int)

    //Pegando os argumentos de entrada
    if len(os.Args)  != 3{
        fmt.Println("Entrada <Numero até qual será contado os primos> <Número de threads>")
        os.Exit(1)
    }
    
    N,err := strconv.Atoi(os.Args[1])
    M,err := strconv.Atoi(os.Args[2])

	if err != nil {
		// Lida com o erro se a conversão falhar
		fmt.Println("Erro na conversão:", err)
		return
	}
    jobs := make(chan int,M) // Faz o canal com o número de threads para passar os números

    // Criando as threads que vão ler os números
    for j:=1; j <= M; j++{
        go threads(jobs,respostas,j)
    }

    //Passando os números para as threads
    for j := 1; j <= N; j++ {
        jobs <- j
    }
    close(jobs)

    for j:= 1 ; j<=M;j++{
        parcial:= <- respostas
        total += parcial
    }
    fmt.Printf("O total de números primos é: %d",total)
}
