
package src;

simple Period
{
    parameters:
        @display("i=block/network;");
//        @signal[tamanhoTurma](type="long");
//        @signal[tamanhoFilaEspera](type="long");
//        @signal[totalPerSemester*](type=simtime_t);
        @signal[dropoutsPerSemester*](type=simtime_t);
        @signal[disapprovalsPerSemester*](type=simtime_t);
        @signal[approvedsPerSemester*](type=simtime_t);
        @signal[graduatesPerSemester*](type=simtime_t);
//        @signal[duracaoTransicaoPeriodo*](type=simtime_t);
        @signal[sizeClass*](type=simtime_t);
        @signal[queueWaitSize*](type=simtime_t);
//        @statistic[tamanhoTurma](title="Turma"; record=stats; unit=ms);
//        @statistic[tamanhoFilaEspera](title="Fila de Espera"; record=stats; unit=ms);
//        @statisticTemplate[totalPerSemesterTemplate](title="Total por Semestre"; record=stats; unit=ms);
        @statisticTemplate[queueWaitSizeTemplate](title="Fila de Espera"; record=stats; unit=ms);
        @statisticTemplate[sizeClassTemplate](title="Tamanho da Turma"; record=stats; unit=ms);
        @statisticTemplate[dropoutsPerSemesterTemplate](title="Evadidos por Semestre"; record=stats; unit=ms);
        @statisticTemplate[disapprovalsPerSemesterTemplate](title="Reprovados por Semestre"; record=stats; unit=ms);
        @statisticTemplate[approvedsPerSemesterTemplate](title="Aprovados por Semestre"; record=stats; unit=ms);
        @statisticTemplate[graduatesPerSemesterTemplate](title="Graduados por Semestre"; record=stats; unit=ms);
//        @statisticTemplate[duracaoTransicaoPeriodoTemplate](title="Dura��o Transi��o Periodo"; record=stats; unit=ms);

        int classCapacity;
        int currentPeriod;
        int numberOfPeriods;
    gates:
        input in[classCapacity*2];
        output out[classCapacity*2];
}


simple Ingress
{
    parameters:
        int classCapacity;
    gates:
        output out[classCapacity];
}

simple Graduation
{
    parameters:
//        @signal[quantidadePeriodos](type="long");
//        @statistic[quantidadePeriodos](title="Quantidade de Periodos"; record=stats; unit=ms);
        int classCapacity;
    gates:
        input in[classCapacity];
}

network Course
{
    @display("bgb=2058.156,1930.248");
//    @signal[totalAlunos](type="long");
//    @signal[totalTurmas](type="long");
//    @signal[totalMatriculas](type="long");
//    @statistic[totalAlunos](title="Total Alunos"; record=stats; unit=ms);
//    @statistic[totalTurmas](title="Total Turmas"; record=stats; unit=ms);
//    @statistic[totalMatriculas](title="Total Matriculas"; record=stats; unit=ms);
    int classCapacity = 70;
    int numberOfPeriods = 9;
    types:
        channel D extends ned.DatarateChannel
        {
            delay = 6.0s;
        }

    submodules:
        ingress: Ingress {
            @display("p=127.644,143.116;i=old/redbox");
            classCapacity = classCapacity;
        }
        periods[numberOfPeriods]: Period {
            currentPeriod = index + 1;
            classCapacity = classCapacity;
            numberOfPeriods = numberOfPeriods;
        }

        graduation: Graduation {
            classCapacity = classCapacity;
            @display("p=131.512,1392.48;i=old/bluebox");
        }

    connections:

        ingress.out[i] --> D --> periods[0].in[i] for i=0..(classCapacity - 1);

        for i=0..(classCapacity - 1), for p=0..(numberOfPeriods - 1) {
            periods[p].out[i] --> D --> periods[p+1].in[i] if p < (numberOfPeriods - 1);
            periods[p].out[i + classCapacity] --> D --> periods[p].in[i + classCapacity] if p < (numberOfPeriods - 1);

        }


        for i=0..(classCapacity - 1) {
            periods[numberOfPeriods - 1].out[i] --> D --> graduation.in[i];
            periods[numberOfPeriods - 1].out[i + classCapacity] --> D --> periods[numberOfPeriods - 1].in[i + classCapacity];
        }

}
