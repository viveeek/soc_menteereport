#include <iostream>
#include <vector>
#include <cmath>
using namespace std ;
struct Candle {
    double open;
    double high;
    double low;
    double close;
};

struct TradeResult {
    double success_rate;
    double average_return;
    int total_trades;
    vector<int> signals;
};

double calculate_rsi(const vector<double> &closes, int index, int period = 14) {
    if (index < period)
        return 50.0;

    double gain = 0.0, loss = 0.0;

    for (int i = index - period + 1; i <= index; ++i) {
        double change = closes[i] - closes[i - 1];
        if (change > 0)
            gain += change;
        else
            loss -= change;
    }

    if (loss == 0) return 100.0;

    double rs = gain / loss;
    return 100.0 - (100.0 / (1.0 + rs));
}
TradeResult run_rsi_strategy(const vector<Candle> &candles, double profit_threshold = 0.01) {
    vector<double> closes;
    for (const auto &c : candles)
        closes.push_back(c.close);

    vector<int> signals(closes.size(), 0);

    int total_trades = 0;
    int profitable_trades = 0;
    double total_return = 0.0;

    bool was_above_60 = false;
    bool was_below_40 = false;

    enum Position { NONE, LONG, SHORT } position = NONE;
    double entry_price = 0.0;

    for (size_t i = 15; i < closes.size(); ++i) {
        double rsi = calculate_rsi(closes, i);
        double change = (closes[i] - closes[i - 1]) / closes[i - 1];
        if (rsi > 60 && change > 0.05) {
            if (!was_above_60) {
                signals[i] = 1;
                was_above_60 = true;
            }
            was_below_40 = false;
        } else if (was_above_60 && rsi < 60) {
            signals[i] = -1;
            was_above_60 = false;
        } else if (rsi < 40 && change < -0.05) {
            if (!was_below_40) {
                signals[i] = -2; 
                was_below_40 = true;
            }
            was_above_60 = false;
        } else if (was_below_40 && rsi > 40) {
            signals[i] = 2; 
            was_below_40 = false;
        }

        if (position == NONE) {
            if (rsi > 60 && change > 0.05) {
                position = LONG;
                entry_price = closes[i];
            } else if (rsi < 40 && change < -0.05) {
                position = SHORT;
                entry_price = closes[i];
            }
        } else if (position == LONG && rsi < 60) {
            double exit_price = closes[i];
            double ret = (exit_price - entry_price) / entry_price;
            total_return += ret;
            if (ret > profit_threshold) ++profitable_trades;
            ++total_trades;
            position = NONE;
        } else if (position == SHORT && rsi > 40) {
            double exit_price = closes[i];
            double ret = (entry_price - exit_price) / entry_price;
            total_return += ret;
            if (ret > profit_threshold) ++profitable_trades;
            ++total_trades;
            position = NONE;
        }
    }
    if (position != NONE) {
        double final_price = closes.back();
        double ret = (position == LONG) ? 
                     (final_price - entry_price) / entry_price : 
                     (entry_price - final_price) / entry_price;
        total_return += ret;
        if (ret > profit_threshold) ++profitable_trades;
        ++total_trades;
    }

    double success_rate = total_trades > 0 ? (double)profitable_trades / total_trades * 100.0 : 0.0;
    double avg_return = total_trades > 0 ? (total_return / total_trades) * 100.0 : 0.0;

    return {success_rate, avg_return, total_trades, signals};
}
int main() {
    vector<Candle> candles = {
        {1.0, 1.2, 0.9, 1.1}, {1.1, 1.3, 1.0, 1.2}, {1.2, 1.4, 1.1, 1.3}, };

    TradeResult result = run_rsi_strategy(candles, 0.01);

    cout << "Success Rate: " << result.success_rate << "%" << endl;
    cout << "Average Return: " << result.average_return << "%" << endl;
    cout << "Total Trades: " << result.total_trades << endl;

}
