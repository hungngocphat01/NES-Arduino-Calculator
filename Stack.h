static const int max_size = 30;

struct sstack {
    int index = -1;
    String data[max_size];

    bool isEmpty() {
        return (index == -1);
    }

    void push(String val) {
        if (index >= max_size - 1) return;
        index++;
        data[index] = val;
    }

    String top() {
        return data[index];
    }

    String pop() {
        String top = data[index];
        index--;
        return top;
    }

    int getSize() {
        return index + 1;
    }

    String operator[] (int i) {
        return data[i];
    }
};

struct fstack {
    int index = -1;
    float data[max_size];

    bool isEmpty() {
        return (index == -1);
    }

    void push(float val) {
        if (index >= max_size - 1) return;
        index++;
        data[index] = val;
    }

    float top() {
        return data[index];
    }

    float pop() {
        float top = data[index];
        index--;
        return top;
    }

    int getSize() {
        return index + 1;
    }

    float operator[] (int i) {
        return data[i];
    }
};
