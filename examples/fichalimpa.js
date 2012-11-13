WS.print("Iniciando processo de captura...");

WS.request({
    verb: 'GET', type: 'json',
    url: 'http://www.fichalimpa.org.br/index.php/main/realizar_consulta',
    data: { estado: 18, municipio: 3333 }, //estado 18 = SP; municipio 3333 = São Paulo
    success: function(resultado) {
        var i, item;
        for(i = 0; i < resultado.data.length; i++) {
            item = resultado.data[i];
            WS.print('Num.: '+ item['1'] +'; Candidato: '+ item['2'] +'; Partido: '+ item['3'] +'; Cargo: '+ item['4']);
        }
        WS.exit();
    },
    error: function(error) {
        WS.print(error);
        WS.exit();
    }
});
