void algoritmoBasico(volatile double ratings[100], volatile double p[40],
		volatile double q[100], int num_iters, int k, int users, int items) {
#pragma HLS ARRAY_PARTITION variable=q complete dim=1
#pragma HLS ARRAY_PARTITION variable=p complete dim=1
#pragma HLS INTERFACE s_axilite port=ratings  bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=p  bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=q  bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=num_iters bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=k bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=users bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=items bundle=CTRL_BUS

	num_iters = 50;

	int u, i, f;
	double escalar = 0.0;
	double error = 0.0;
	float lambda = 0.055;
	float gamma = 0.01;
	k = 15;
	users = 1000; //Max_users
	items = 1000; //Max_items

	for (int iter = 0; iter < num_iters; iter++) {

		for (u = 0; u < users; u++) {
			for (i = 0; i < items; i++) {
				for (f = 0; f < k; f++) {
#pragma HLS PIPELINE
					if (ratings[u * items + i] != -1) {
						escalar += p[u * k + f] * q[i * k + f];

					}
				}
				if (ratings[u * items + i] != -1) {
					error = ratings[u * items + i] - (escalar);

				}
				escalar = 0.0;

				for (f = 0; f < k; f++) {
#pragma HLS PIPELINE
					if (ratings[u * items + i] != -1) {

						p[u * k + f] = p[u * k + f]
								+ gamma
										* (error * q[i * k + f]
												- lambda * p[u * k + f]);

					}
				}

			}

		}

		for (i = 0; i < items; i++) {
			for (u = 0; u < users; u++) {
				for (f = 0; f < k; f++) {
#pragma HLS PIPELINE
					if (ratings[u * items + i] != -1)
						escalar += p[u * k + f] * q[i * k + f];

				}
				if (ratings[u * items + i] != -1)
					error = ratings[u * items + i] - (escalar);
				escalar = 0.0;

				for (f = 0; f < k; f++) {
#pragma HLS PIPELINE
					if (ratings[u * items + i] != -1)
						q[i * k + f] = q[i * k + f]
								+ gamma
										* (error * p[u * k + f]
												- lambda * q[i * k + f]);

				}

			}

		}

	}

}
