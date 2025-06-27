#include <iostream>
#include <vector>
using namespace std;

struct Candle {
    double open;
    double high;
    double low;
    double close;
};

double calculate_sma(const vector<double>& data, int i, int period) {
    if (i + 1 < period) return -1.0;
    double sum = 0.0;
    for (int j = i - period + 1; j <= i; ++j)
        sum += data[j];
    return sum / period;
}

void run_sma(const vector<Candle>& candles, int sma_short, int sma_long) {
    vector<double> closes;
    for (const auto& c : candles)
        closes.push_back(c.close);

    bool in_position = false;
    double entry_price = 0.0;
    double total_return = 0.0;
    int trade_count = 0;

    for (size_t i = 0; i < closes.size(); ++i) {
        double sma_fast = calculate_sma(closes, i, sma_short);
        double sma_slow = calculate_sma(closes, i, sma_long);

        if (sma_fast < 0 || sma_slow < 0) continue;

        if (!in_position && sma_fast > sma_slow) {
            in_position = true;
            entry_price = closes[i];
            cout << "Buy at: " << closes[i] << "\n";
        }
        else if (in_position && sma_fast < sma_slow) {
            double ret = (closes[i] - entry_price) / entry_price;
            total_return += ret;
            trade_count++;
            in_position = false;
            cout << "Sell at: " << closes[i] << " | Return: " << ret * 100 << "%\n";
        }
    }

    if (in_position) {
        double ret = (closes.back() - entry_price) / entry_price;
        total_return += ret;
        trade_count++;
        cout << "Final sell at: " << closes.back() << " | Return: " << ret * 100 << "%\n";
    }

    cout << "Total Trades: " << trade_count << "\n";
    cout << "Average Return: " << (trade_count > 0 ? (total_return / trade_count * 100) : 0.0) << "%\n";
}

int main() {
    vector<Candle> candles = {
        {1,1.1,0.9,1.0}, {1,1.2,0.95,1.1}, {1.1,1.3,1.0,1.2}, {1.2,1.4,1.1,1.3},
        {1.3,1.5,1.2,1.4}, {1.4,1.6,1.3,1.5}, {1.5,1.7,1.4,1.6}, {1.6,1.8,1.5,1.7},
        {1.7,1.9,1.6,1.8}, {1.8,2.0,1.7,1.9}, {1.9,2.1,1.8,2.0}, {2.0,2.2,1.9,2.1},
        {2.1,2.3,2.0,2.2}, {2.2,2.4,2.1,2.3}, {2.3,2.5,2.2,2.4}, {2.4,2.6,2.3,2.5},
        {2.5,2.7,2.4,2.6}, {2.6,2.8,2.5,2.7}, {2.7,2.9,2.6,2.8}, {2.8,3.0,2.7,2.9},
        {2.9,3.1,2.8,3.0}, {3.0,3.2,2.9,3.1}, {3.1,3.3,3.0,3.2}, {3.2,3.4,3.1,3.3},
        {3.3,3.5,3.2,3.4}, {3.4,3.6,3.3,3.5}, {3.5,3.7,3.4,3.6}, {3.6,3.8,3.5,3.7},
        {3.7,3.9,3.6,3.8}, {3.8,4.0,3.7,3.9}, {3.9,4.1,3.8,4.0}, {4.0,4.2,3.9,4.1},
        {4.1,4.3,4.0,4.2}, {4.2,4.4,4.1,4.3}, {4.3,4.5,4.2,4.4}, {4.4,4.6,4.3,4.5},
        {4.5,4.7,4.4,4.6}, {4.6,4.8,4.5,4.7}, {4.7,4.9,4.6,4.8}, {4.8,5.0,4.7,4.9},
        {4.9,5.1,4.8,5.0}, {5.0,5.2,4.9,5.1}, {5.1,5.3,5.0,5.2}, {5.2,5.4,5.1,5.3},
        {5.3,5.5,5.2,5.4}, {5.4,5.6,5.3,5.5}, {5.5,5.7,5.4,5.6}, {5.6,5.8,5.5,5.7},
        {5.7,5.9,5.6,5.8}, {5.8,6.0,5.7,5.9}, {5.9,6.1,5.8,6.0}, {6.0,6.2,5.9,6.1},
        {6.1,6.3,6.0,6.2}, {6.2,6.4,6.1,6.3}, {6.3,6.5,6.2,6.4}, {6.4,6.6,6.3,6.5},
        {6.5,6.7,6.4,6.6}, {6.6,6.8,6.5,6.7}, {6.7,6.9,6.6,6.8}, {6.8,7.0,6.7,6.9},
        {6.9,7.1,6.8,7.0}, {7.0,7.2,6.9,7.1}, {7.1,7.3,7.0,7.2}, {7.2,7.4,7.1,7.3}
    };

    int sma_short = 50;
    int sma_long = 200;

    run_sma(candles, sma_short, sma_long);

}
