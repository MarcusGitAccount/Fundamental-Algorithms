#ifndef TRAVERSAL_H
# define TRAVERSAL_H

typedef enum { WHITE, GREY, BLACK } Color_t;

struct NodeTimes {
	int discover, end;

	NodeTimes() {
		discover = end = -1;
	}
};

struct Traversal {


	Color_t* colors;
	NodeTimes* times;
	int* parents;

	Traversal(int n) {
		this->colors = new Color_t[n];
		this->times = new NodeTimes[n];
		this->parents = new int[n];

		for (int i = 0; i < n; i++) {
			this->colors[i] = WHITE;
			this->parents[i] = -1;
			this->times[i].discover = this->times[i].end = -1;
		}
	}

	~Traversal() {
		delete this->colors;
		delete this->times;
		delete this->parents;
	}
};

#endif // !TRAVERSAL_H
