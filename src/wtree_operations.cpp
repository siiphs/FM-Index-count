#include "../include/wtree-lib/wtree.hpp"

namespace CDS_UDEC {
    uint32_t WaveletTree::access(uint32_t index) {
        int h = height;
        uint32_t ele = 0;
        Node *aux = root;
        while (aux != nullptr) {
            if (aux -> representation.access(index) == 0) {
                index = aux -> representation.rank0(index);
                aux = aux -> childs[0];
            }
            else {
                index = aux -> representation.rank1(index);
                aux = aux -> childs[1];
                ele |= (1 << h);
            }
            h--;
        }
        return ele;
    }

    uint32_t WaveletTree::rank(uint32_t symbol, uint32_t index) {
        int h = height;
        Node *aux = root;
        while (aux != nullptr) {
            if (symbol & (1 << h)) {
                index = aux -> representation.rank1(index);
                aux = aux -> childs[1];
            }
            else {
                index = aux -> representation.rank0(index);
                aux = aux -> childs[0];
            }
            h--;
        }
        return index;
    }

    uint32_t WaveletTree::getSize() {
        return size;
    }

    uint64_t WaveletTree::size_in_bytes() {
        if (root == nullptr) return 0;
        return _size_bytes_recursive(root);
    }

    uint64_t WaveletTree::_size_bytes_recursive(Node* node) {
        if (node == nullptr) return 0;
        uint64_t bytes = sizeof(Node);
        bytes += node->representation.size_in_bytes();
        bytes += _size_bytes_recursive(node->childs[0]);
        bytes += _size_bytes_recursive(node->childs[1]);
        return bytes;
    }
}