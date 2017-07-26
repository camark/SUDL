#ifndef SEQ_LOSS_LAYER_H
#define SEQ_LOSS_LAYER_H

#include "layer.h"

namespace sub_dl {

class SeqActiveLayer : public Layer {

public:
	
	SeqActiveLayer() {
		_type = SEQ_ACT;
	}

	void _forward(Layer* pre_layer) {
		std::vector<matrix_double>().swap(_data);
		_output_dim = pre_layer->_output_dim;
		_input_dim = pre_layer->_input_dim;
		_seq_len = pre_layer->_seq_len;
		for (int t = 0; t < _seq_len; t++) {
			matrix_double val = sigmoid_m(pre_layer->_data[t]);
			_data.push_back(val);
		}
		_pre_layer = pre_layer;

	}

	void _backward(Layer* nxt_layer) {
		std::vector<matrix_double>().swap(_errors);
		for (int t = _seq_len - 1; t >= 0; t--) {
			matrix_double error = nxt_layer->_errors[t].dot_mul(
				sigmoid_m_diff(_data[t]));
			_errors.push_back(error);
		}
		std::reverse(_errors.begin(), _errors.end());
	}
	
	void display() {
		std::cout << "-----------seq active alyer-------" << std::endl;
		for (int i = 0; i < _seq_len; i++) {
			_data[i]._display("_data.push_back");
		}
		for (int i = 0; i < _seq_len; i++) {
			_errors[i]._display("error");
		}
	}
	void _update_gradient(int opt_type, double learning_rate) {
	}

};

class SeqLossLayer : public Layer {

public:
	matrix_double _label;

	SeqLossLayer(const matrix_double& label) {
		_label = label;
		_type = SEQ_LOSS;
	}

	void _forward(Layer* pre_layer) {		
		std::vector<matrix_double>().swap(_data);
		_seq_len = pre_layer->_seq_len;
		_output_dim = pre_layer->_output_dim;
		_input_dim = pre_layer->_input_dim;
		
		for (int t = 0; t < _seq_len; t++) {
			matrix_double diff_val = pre_layer->_data[t] - _label._R(t);
			_data.push_back((diff_val.dot_mul(diff_val)) * 0.5);
		}
		_pre_layer = pre_layer;
	}

	void _backward(Layer* nxt_layer) {
		std::vector<matrix_double>().swap(_errors);

		for (int t = _seq_len - 1; t >= 0; t--) {
			matrix_double error = (_pre_layer->_data[t] - _label._R(t));
			_errors.push_back(error);
		}
		std::reverse(_errors.begin(), _errors.end());

	}

	void display() {
		std::cout << "--------seq loss layer ----------" << std::endl;
		_label._display("label");
		for (int t = 0; t < _seq_len; t++) {
			_data[t]._display("data");
		}
		for (int t = 0; t < _seq_len; t++) {
			_errors[t]._display("error");
		}
	}

	void _update_gradient(int opt_type, double learning_rate) {
	}
};

}

#endif