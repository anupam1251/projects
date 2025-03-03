#include <iostream>
#include <unordered_map>
#include <map>


class Order {
	size_t m_orderId;
	std::string m_symbol;
	std::string m_Ordertype;
	double m_orderPrice;
	size_t m_quantity;

public:

	Order(size_t id, std::string symbol, std::string type, double price, size_t quantity) :
		m_orderId(id), m_symbol(symbol), m_Ordertype(type), m_orderPrice(price), m_quantity(quantity) {

	}

	size_t getOrderId() const { return m_orderId; }
	std::string getSymbol() const { return m_symbol; }
	std::string getOrderType() const { return m_Ordertype; }
	double getOrderPrice() const { return m_orderPrice; }
	size_t getQuantity() const { return m_quantity; }

};

class Tick {
	std::string m_symbol;
	double m_marketPrice;
	std::string m_Timestamp;

public:
	
	Tick(std::string symbol, double marketPrice, std::string timeStamp) :
		m_symbol(symbol), m_marketPrice(marketPrice), m_Timestamp(timeStamp) {

	}

	std::string getSymbol() const { return m_symbol; }
	double getCurrentMarketPrice() const { return m_marketPrice; }
	std::string getTimeStamp() const { return m_Timestamp; }

};


class ExecuteOrder {
	std::unordered_map<std::string, std::multimap<double, Order>> m_mapbuyLimitOrder;
	std::unordered_map<std::string, std::multimap<double, Order>> m_mapbuyStopLimitOrder;
public:

	void addOrder(const Order& order) {
		if (order.getOrderType() == "BL") {
			m_mapbuyLimitOrder[order.getSymbol()].emplace(order.getOrderPrice(), order);
		}
		else if (order.getOrderType() == "SL") {
			m_mapbuyStopLimitOrder[order.getSymbol()].emplace(order.getOrderPrice(), order);
		}
	}

	void newTick(const Tick& tick) {
		std::string symbol = tick.getSymbol();
		double cmp = tick.getCurrentMarketPrice();
		std::string timestamp = tick.getTimeStamp();

		if (m_mapbuyLimitOrder.find(symbol) != m_mapbuyLimitOrder.end()) {
			auto& orders = m_mapbuyLimitOrder[symbol];
			auto it = orders.begin();
			while(it != orders.end()){
				if (it->first >= cmp) {
					std::cout << "Order "<<it->second.getOrderId()<<" executed at " << timestamp << ", Price: " << cmp << ", Quantity : " <<
						it->second.getQuantity() << '\n';
					it = orders.erase(it);
				}
				else {
					++it;
				}
			}
			if (orders.empty()) {
				m_mapbuyLimitOrder.erase(symbol);
			}
		}

		if (m_mapbuyStopLimitOrder.find(symbol) != m_mapbuyStopLimitOrder.end()) {
			auto& orders = m_mapbuyStopLimitOrder[symbol];
			auto it = orders.begin();
			while (it != orders.end()) {
				if (it->first <= cmp) {
					std::cout << "Order "<<it->second.getOrderId()<<" executed at " << timestamp << ", Price: " << cmp << ", Quantity : " <<
						it->second.getQuantity() << '\n';
					it = orders.erase(it);
				}
				else {
					++it;
				}
			}
			if (orders.empty()) {
				m_mapbuyStopLimitOrder.erase(symbol);
			}
		}

	}
};
int main() {


	ExecuteOrder orderBook;
	// Test case 1
	orderBook.addOrder(Order(1,"AAPL", "BL", 100, 10));
	orderBook.addOrder(Order(2,"AAPL", "BL", 105, 5));
	orderBook.addOrder(Order(3,"AAPL", "BL", 102, 15));
	orderBook.addOrder(Order(4,"AAPL", "BL", 98, 15));
	orderBook.addOrder(Order(5,"GOOG", "BL", 200, 10));
	orderBook.addOrder(Order(6,"GOOG", "BL", 201, 15));
	orderBook.addOrder(Order(7,"GOOG", "BL", 198, 10));


	// Test Case 2
/*
	orderBook.addOrder(Order(1,"AAPL", "SL", 110, 10));
	orderBook.addOrder(Order(2,"AAPL", "SL", 108, 5));
	orderBook.addOrder(Order(3,"GOOG", "SL", 210, 10));
	orderBook.addOrder(Order(4,"GOOG", "SL", 205, 20)); */


	orderBook.newTick(Tick("AAPL", 104, "09:00"));
	orderBook.newTick(Tick("AAPL", 102, "09:01"));
	orderBook.newTick(Tick("AAPL", 98, "09:02"));
	orderBook.newTick(Tick("GOOG", 200, "09:03"));
	orderBook.newTick(Tick("AAPL", 110, "09:05"));
	orderBook.newTick(Tick("GOOG", 210, "09:06"));


}