
package src;

simple Periodo
{
    parameters:
        @display("i=block/network;");
        @signal[tamanhoTurma](type="long");
        @signal[tamanhoFilaEspera](type="long");
        @signal[quantidadeEvadidosGeral](type="long");
        @signal[quantidadeReprovadosGeral](type="long");
        @signal[quantidadeGeral](type="long");
        @signal[totalPorSemestre*](type=simtime_t);
        @signal[evadidosPorSemestre*](type=simtime_t);
        @signal[reprovadosPorSemestre*](type=simtime_t);
        @signal[graduadosPorSemestre*](type=simtime_t);
        @signal[duracaoTransicaoPeriodo*](type=simtime_t);
        @statistic[tamanhoTurma](title="Turma"; record=stats; unit=ms);
        @statistic[tamanhoFilaEspera](title="Fila de Espera"; record=stats; unit=ms);
        @statistic[quantidadeEvadidosGeral](title="Evadidos Geral"; record=stats; unit=ms);
        @statistic[quantidadeReprovadosGeral](title="Reprovados Geral"; record=stats; unit=ms);
        @statisticTemplate[totalPorSemestreTemplate](title="Total por Semestre"; record=stats; unit=ms);
        @statisticTemplate[evadidosPorSemestreTemplate](title="Evadidos por Semestre"; record=stats; unit=ms);
        @statisticTemplate[reprovadosPorSemestreTemplate](title="Reprovados por Semestre"; record=stats; unit=ms);
        @statisticTemplate[graduadosPorSemestreTemplate](title="Graduados por Semestre"; record=stats; unit=ms);
        @statisticTemplate[duracaoTransicaoPeriodoTemplate](title="Duração Transição Periodo"; record=stats; unit=ms);

        int capacidadeTurma;
        int numero;
        int numeroPeriodos;
    gates:
        input entrada[capacidadeTurma*2];
        output saida[capacidadeTurma*2];
}


simple Inicio
{
    parameters:
        int capacidadeTurma;
    gates:
        output saida[capacidadeTurma];
}

simple Fim
{
    parameters:
        @signal[quantidadePeriodos](type="long");
        @statistic[quantidadePeriodos](title="Quantidade de Periodos"; record=stats; unit=ms);
        int capacidadeTurma;
    gates:
        input entrada[capacidadeTurma];
}

network Curso
{
    @display("bgb=2058.156,1930.248");
    @signal[totalEvadidos](type="long");
    @signal[totalAlunos](type="long");
    @signal[totalTurmas](type="long");
    @signal[totalMatriculas](type="long");
    @signal[totalFormados](type="long");
    @statistic[totalFormados](title="Formados"; record=stats; unit=ms);
    @statistic[totalEvadidos](title="Total Evadidos"; record=stats; unit=ms);
    @statistic[totalAlunos](title="Total Alunos"; record=stats; unit=ms);
    @statistic[totalTurmas](title="Total Turmas"; record=stats; unit=ms);
    @statistic[totalMatriculas](title="Total Matriculas"; record=stats; unit=ms);
    int capacidadeTurma = 200;
    int numeroPeriodos = 10;
    types:
        channel D extends ned.DatarateChannel
        {
            delay = 6.0s;
        }

    submodules:
        inicio: Inicio {
            @display("p=127.644,143.116;i=old/redbox");
            capacidadeTurma = capacidadeTurma;
        }
        periodos[numeroPeriodos]: Periodo {
            numero = index + 1;
            capacidadeTurma = capacidadeTurma;
            numeroPeriodos = numeroPeriodos;
        }

        fim: Fim {
            capacidadeTurma = capacidadeTurma;
            @display("p=131.512,1392.48;i=old/bluebox");
        }

    connections:

        inicio.saida[i] --> D --> periodos[0].entrada[i] for i=0..(capacidadeTurma - 1);

        for i=0..(capacidadeTurma - 1), for p=0..(numeroPeriodos - 1) {
            periodos[p].saida[i] --> D --> periodos[p+1].entrada[i] if p < (numeroPeriodos - 1);
            periodos[p].saida[i + capacidadeTurma] --> D --> periodos[p].entrada[i + capacidadeTurma] if p < (numeroPeriodos - 1);

        }


        for i=0..(capacidadeTurma - 1) {
            periodos[numeroPeriodos - 1].saida[i] --> D --> fim.entrada[i];
            periodos[numeroPeriodos - 1].saida[i + capacidadeTurma] --> D --> periodos[numeroPeriodos - 1].entrada[i + capacidadeTurma];
        }

}
