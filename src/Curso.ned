
package src;

simple Periodo
{
    parameters:
        @display("i=block/network;");
        @signal[tamanhoTurma](type="long");
        @signal[tamanhoFilaEspera](type="long");
//        @signal[evadidos](type="long");
//        @signal[retidos](type="long");
//        @signal[graduados](type="long");
//        @signal[desvinculados](type="long");
        @signal[quantidadeEvadidosGeral](type="long");
        @signal[quantidadeRetidosGeral](type="long");
        @signal[evadidosPorPeriodo*](type=simtime_t);
        @signal[retidosPorPeriodo*](type=simtime_t);
        @signal[evadidosPorSemestre*](type=simtime_t);
        @signal[retidosPorSemestre*](type=simtime_t);
        @signal[graduadosPorSemestre*](type=simtime_t);
        @signal[evadidosSemestrePeriodo*](type=simtime_t);
        @signal[retidosSemestrePeriodo*](type=simtime_t);
        @signal[totalSemestrePeriodo*](type=simtime_t);
        @statistic[evadidosPorPeriodoTemplate](title="Evadidos por Periodo";record=stats;unit=ms);
        @statistic[retidosPorPeriodoTemplate](title="Retidos por Periodo";record=stats;unit=ms);
        @statistic[tamanhoTurma](title="Turma"; record=stats; unit=ms);
        @statistic[tamanhoFilaEspera](title="Fila de Espera"; record=stats; unit=ms);
        @statistic[quantidadeEvadidosGeral](title="Evadidos Geral"; record=stats; unit=ms);
        @statistic[quantidadeRetidosGeral](title="Retidos Geral"; record=stats; unit=ms);
        @statisticTemplate[evadidosPorPeriodoTemplate](title="Evadidos por Periodo";record=stats;unit=ms);
		@statisticTemplate[retidosPorPeriodoTemplate](title="Retidos por Periodo";record=stats;unit=ms);
		@statisticTemplate[evadidosPorSemestreTemplate](title="Evadidos por Semestre";record=stats;unit=ms);
		@statisticTemplate[retidosPorSemestreTemplate](title="Retidos por Semestre";record=stats;unit=ms);
		@statisticTemplate[graduadosPorSemestreTemplate](title="Graduados por Semestre";record=stats;unit=ms);
        @statisticTemplate[evadidosSemestrePeriodoTemplate](title="Evadidos Semestre Periodo";record=stats;unit=ms);
        @statisticTemplate[retidosSemestrePeriodoTemplate](title="Retidos Semestre Periodo";record=stats;unit=ms);
        @statisticTemplate[totalSemestrePeriodoTemplate](title="Total Semestre Periodo";record=stats;unit=ms);
		
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
    @signal[totalFormados](type="long");
   	@statistic[totalFormados](title="Formados"; record=stats; unit=ms);
    @statistic[totalEvadidos](title="Total Evadidos"; record=stats; unit=ms);
    @statistic[totalAlunos](title="Total Alunos"; record=stats; unit=ms);
    int capacidadeTurma = 200;
    int numeroPeriodos = 9;
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
//        p1: Periodo {
//            numero = 1;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=123.776,440.952");
//        }
//        p2: Periodo {
//            numero = 2;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=475.764,448.688");
//        }
//        p3: Periodo {
//            numero = 3;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=847.092,448.688");
//        }
//        p4: Periodo {
//            numero = 4;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=1210.684,456.424");
//        }
//        p5: Periodo {
//            numero = 5;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=1628.428,460.292");
//        }
//        p6: Periodo {
//            numero = 6;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=1628.428,1009.548");
//        }
//        p7: Periodo {
//            numero = 7;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=1202.948,1013.416");
//        }
//        p8: Periodo {
//            numero = 8;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=800.676,1009.548");
//        }
//        p9: Periodo {
//            numero = 9;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=452.556,1013.416");
//        }
//        p10: Periodo {
//            numero = 10;
//            capacidadeTurma = capacidadeTurma;
//            @display("p=135.38,1013.416");
//        }
        fim: Fim {
            capacidadeTurma = capacidadeTurma;
            @display("p=131.512,1392.48;i=old/bluebox");
        }

    connections:

        inicio.saida[i] --> D --> periodos[0].entrada[i] for i=0..(capacidadeTurma - 1);

		for i=0..(capacidadeTurma - 1), 			
			for p=0..(numeroPeriodos - 1) {
				
    			periodos[p].saida[i] --> D --> periodos[p+1].entrada[i] if p < (numeroPeriodos - 1);
    			periodos[p].saida[i + capacidadeTurma] --> D --> periodos[p].entrada[i + capacidadeTurma] if p < (numeroPeriodos - 1);   			
    			
		}
		
		
		for i=0..(capacidadeTurma - 1){
    		periodos[numeroPeriodos - 1].saida[i] --> D --> fim.entrada[i];
        	periodos[numeroPeriodos - 1].saida[i + capacidadeTurma] --> D --> periodos[numeroPeriodos - 1].entrada[i + capacidadeTurma];
		}




//        for i=0..(capacidadeTurma - 1) {
//            inicio.saida[i] --> D --> p1.entrada[i];
//
//            p1.saida[i] --> D --> p2.entrada[i];
//            p1.saida[i+capacidadeTurma] --> D --> p1.entrada[i+capacidadeTurma];
//
//            p2.saida[i] --> D --> p3.entrada[i];
//            p2.saida[i+capacidadeTurma] --> D --> p2.entrada[i+capacidadeTurma];
//
//            p3.saida[i] --> D --> p4.entrada[i];
//            p3.saida[i+capacidadeTurma] --> D --> p3.entrada[i+capacidadeTurma];
//
//            p4.saida[i] --> D --> p5.entrada[i];
//            p4.saida[i+capacidadeTurma] --> D --> p4.entrada[i+capacidadeTurma];
//
//            p5.saida[i] --> D --> p6.entrada[i];
//            p5.saida[i+capacidadeTurma] --> D --> p5.entrada[i+capacidadeTurma];
//
//            p6.saida[i] --> D --> p7.entrada[i];
//            p6.saida[i+capacidadeTurma] --> D --> p6.entrada[i+capacidadeTurma];
//
//            p7.saida[i] --> D --> p8.entrada[i];
//            p7.saida[i+capacidadeTurma] --> D --> p7.entrada[i+capacidadeTurma];
//
//            p8.saida[i] --> D --> p9.entrada[i];
//            p8.saida[i+capacidadeTurma] --> D --> p8.entrada[i+capacidadeTurma];
//
//            p9.saida[i] --> D --> p10.entrada[i];
//            p9.saida[i+capacidadeTurma] --> D --> p9.entrada[i+capacidadeTurma];
//
//            p10.saida[i] --> D --> fim.entrada[i];
//            p10.saida[i+capacidadeTurma] --> D --> p10.entrada[i+capacidadeTurma];
//
//        }


}
