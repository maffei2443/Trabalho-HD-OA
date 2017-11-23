// Obs: parte do pre-suposto que existe algum espaço livre.
// Obs2 : NAO modifica o ESTADO de FatEnt
ui FatTable :: alloc_first(){
	for(int l = 0; l < 5; l++)	 			// tenta inserir na primeira superficie
		for(int k = 0; k < 10; k++)		// do primeiro cilindro que encontrar
			for(int i = 0; i < 15; i++)			// que tenha um cluster livre
				if( this->clusters[ i + k * 75 + l * 15 ] == false)
					return (i + k * 75 + l * 15);
}

ui FatTable :: alloc_nxt(ui cluster, ui track, ui cylinder){
	ui onde;
	if(alloc == true){cout << "FatTable :: alloc_nxt throw\n";throw 999;}

	ui mod = cluster;
	bool first = true;
	bool alloc = false;	

	int i, j, k;
	k = cylinder;
	int debug = 0;		// throw se dar mais de 749 iteracoes :/
	for(i = cluster; alloc == false;){		

		for(j = track + 1; alloc == false and j != 0; j++){
			j %= 5;
			if(j == 0){				// Passou pela ultima trilha; passar ao proximo cluster 
									// da primeira trilha do mesmo cilindro.
					break;
			}
			else if(this->clusters[i + 15 * j + 75 * k] == false){
				alloc = true;
				onde = (i + 15 * j + 75 * k);
			}
		}
		track = -1;	// Para sempre começar pela trilha mais em cima, com exceçao claro da primeira na qual
					// tenta-se inserir na TRILHA SEGUINTE aa passada por parametro.
					// Passar para o proximo cluster, pois nao inseriu em nenhuma das trilhas abaixo.
		i++;		// Iterar AQUI!! Senao, zoa a questao da comparacao com 'mod'.
		i %= 15;	
		if(i == mod){		// FINALMENTE deu a primeira volta.
			mod = -1;		// Para nao mais entrar aqui.
			first = false;	// Apenas para nao dar voltas a toa :)
			i = 0;			// Zera o cluster
		}
		else if(i == 0 and !first ){	// Nao eh mais a primeira volta. Checar se o cluster zerou; se sim, trocar de cilindro.
				k++;		
				k %= 10;	// Para nao estourar o cilindro.
		}
		
		debug++;
		if(debug > 750)	{cout << "Erro em FatTable :: alloc_nxt" << debug << "\n"}
	}
	return onde;
}

vector<ui> FatTable :: alloc_space(const string& file, ui size){
	cout << "Clusters livres ANTES:: " << (750 - usado) << endl;
	cout << "Size recebido : "<< size << endl;
	ui qtt = (int) ceil((float)((int)size) / (int)Qtt::CLUSTER);		// Pega o nº do cluster necessarios
	cout << "Em termos de clusters: " << qtt << endl;
	
	// Checa para ver se ainda cabe nos clusters restantes.
	if(qtt > this->usado == 750 )	{cout << "!!!!!!!!!! HD nao suporta mais tal arquivo !!!!!!!!!!" << endl;	return vector<ui>(0);}

	//	Encontra o cluster adequado para a primeira posicao.
	// CLuster encontrado.

	// Checa se existe arquivo de mesmo nome. Se sim, RETURN.
	for(auto it = this->fatlist.cbegin(); it != fatlist.cend(); it++){
		auto aux = (*it);
		string old_file = aux.g_file_name();
		if( old_file == file ){
			cout << "Erro : arquivo de mesmo nome jah existente.\n";
			return vector<ui>(0);
		}
	}
	/////////////////////////////////////////////////////////

	ui first = alloc_first();
	
	vector<ui> lista(0);				// Lista contendo os cluster ussados

	if(qtt < 2){	// Arquivo vazio ou com tam < Qtt :: CLUSTER
		lista.push_back( first );	// Lista de posicoes do cluster do arquivo recebido
		if(unused.size() == 0)
			this->full = true;
		fatent[first] = FatEnt(true, true, -1);	// used = true, eof = true, next = -1 (valor qqer)
		cout << "^^^^^ Arquivo \"" << file << "\" ocupa apenas um cluster\n";
		cout << first << endl;
		return lista;
	}

	qtt--;

	ui next = first + 15;
	next %= 750;	// Para caso de a volta no cilindro.
	
	ui where_cylinder = which_cylinder(next);
	ui where_treck = which_track(next);
	ui where_cluster = which_cluster(next);

	int i, j;
	for(i = where_cluster; qtt > 0 ; i%=15){	// Percorre a trilha.

		for( j = where_track; j < 5 and qtt > 0; j++){	// Percorre em peh.
			if( this->clusters[ where_cluster + j * 15 + where_cylinder * 75 ] == false){
				fatent[first] = FatEnt(true, false, next);
				qtt--;
				first = next;
			}
			next = alloc_nxt(where_cluster, where_treck, where_cylinder);

			where_cylinder = which_cylinder(next);
			where_treck = which_track(next);
			where_cluster = next - where_cylinder * 150 - where_treck * 15;
		}
	}
	
	ui next = where_cluster + where_treck * 15 + where_cylinder * 150;
	bool change_cylinder = false;
	for(;qtt > 0; qtt--){	// Quando qtt == 1, eh o ultimo setor a ser inserido

		where_treck++;	// Tentar colocar na proxima trilha
		where_treck %= 5;

		if(where_treck == 0)	// Tentar colocar no proximo cluster  da trilha 0.
			where_cluster++;
		where_cluster %= 15;

		if(where_cluster == 0 and where_treck == 0)	// Passar pro proximo cilindro.
			where_cylinder++;
		where_cylinder %= 10;
		next = where_cluster + where_treck * 15 + where_cylinder * 150;

		if( unused.count( next ) != 0 ){// Nao usou o proximo proposto insere ele.
			lista.push_back(first);
			this->unused.erase(first);
			this->used.insert(first);
			fatent[first] = FatEnt(true, false, next);
			first = next;
		}

	}

	// Inserir o ultimo setor; calcular também seu tamanho exato

	fatent[first] = FatEnt(true, true, -1);
	lista.push_back( first );
	this->unused.erase( first );
	this->used.insert( first );
	// Todo os cluster na tbela fat adequadamente enumerados
	for(auto it = lista.cbegin(); it != lista.cend(); it++)
		cout << "Cluster ocupado : " << (*it) << endl;
	return lista;
	getchar();
	getchar();
}
