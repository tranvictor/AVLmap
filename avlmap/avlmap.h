#ifndef AVL_MAP_H
#define AVL_MAP_H

#include <functional>
#include <stdexcept>
#include <iostream>
#if __cplusplus >= 201103L
# define NOEXCEPT noexcept
# include <initializer_list>
# include <tuple>
#else
# define NOEXCEPT
#endif

template <typename key,
typename T,
typename compare = std::less<key>,
typename alloc = std::allocator<std::pair<const key, T> > >
class avl_tree{
    
	friend class node;
    friend class value_compare;
    
public:
    typedef key                                          key_type;
    typedef T										     mapped_type;
    typedef std::pair<const key, T>                      value_type;
    typedef compare                                      key_compare;
    typedef alloc                                        allocator_type;
    typedef typename allocator_type::pointer             pointer;
    typedef typename allocator_type::const_pointer       const_pointer;
    typedef typename allocator_type::reference           reference;
    typedef typename allocator_type::const_reference     const_reference;
    typedef size_t                                       size_type;
    
    class value_compare : public std::binary_function<value_type, value_type, bool>
    {
    protected:
        compare comp;
        value_compare (compare c) : comp(c) {}  // constructed with tree's comparison object
    public:
        bool operator() (const value_type& x, const value_type& y) const
        {
            return comp(x.first, y.first);
        }
    };
    
private:
    struct node
    {
        value_type* value;
		size_type height;
        node* parent; // needed by find(), insert(), etc. (the rb tree uses it too by the way)
        node* left;
        node* right;
        int balance;
        node()
        {
			value = 0;
            parent = 0;
            left = 0;
            right = 0;
            height = 1;
            balance = 0;
        }
        
        node(value_type &v, node &p, node &l, node &r){
        	value = v;
        	parent = p;
        	left = l;
        	right = r;
        	update_balance();
        }
        
        size_t left_height(){
        	if (left!=0) return left->height; else return 0;
        }
        size_t right_height(){
            if (right!=0) return right->height; else return 0;
        }
        
        void __print(){
        	std::cout << value->first << "<->" << value->second << "-height:" << height;
        	if (parent != 0) std::cout << "-parent:" << parent->value->first
                << "-left:" << (this==parent->left);
        	std::cout << std::endl;
        	if (left != 0 && left->height > 0) left->__print();
        	if (right != 0 && right->height > 0) right->__print();
        }
        
        void update_balance(){
        	size_type lh = 0;
        	size_type rh = 0;
        	if (left) lh = left->height;
        	if (right) rh = right->height;
        	height = std::max(lh, rh) + 1;
        	balance = (int)(lh - rh);
        }
        
        ~node()
        {
			if (value != 0) delete value;
        }
    };
    
public:
    class const_iterator;
    class iterator
    :public std::iterator<std::bidirectional_iterator_tag, node>
    {
        friend class avl_tree;
		friend class const_iterator;
    public:
        
        iterator(node* n):node_(n){}
        
        iterator():node_(0){}
        
        ~iterator(){}
        
        iterator(const iterator& it)
        {
            node_ = it.node_;
        }
        
        iterator& operator=(const iterator& it)
        {
            node_ = it.node_;
            return *this;
        }
        
        iterator& operator++()
        {
            if(node_->right != 0)
            {
                node_ = node_->right;
                while(node_->left != 0)
                    node_ = node_->left;
            }
            else
            {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->left == temp) break;
                } while(node_->parent != 0);
            }
            return *this;
        }
        
        iterator operator++(int)
        {
            iterator temp1 = *this;
            if(node_->right != 0)
            {
                node_ = node_->right;
                while(node_->left != 0)
                    node_ = node_->left;
            }
            else
            {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->left == temp) break;
                } while(node_->parent != 0);
            }
            return temp1;
        }
        
        iterator& operator--() {
            if (node_ -> left != 0){
                node_ = node_->left;
                while(node_->right != 0) node_ = node_->right;
            } else {
                node* temp;
                do {
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->right == temp) break;
                } while(node_->parent != 0 && node_ == node_->parent->left);
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator temp1 = *this;
            if (node_ -> left != 0){
                node_ = node_->left;
                while(node_->right != 0) node_ = node_->right;
            } else {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->right == temp) break;
                } while(node_->parent != 0 && node_ == node_->parent->left);
            }
            return temp1;
        }
        
        typename avl_tree::reference operator*() const
        {
            return *(node_->value);
        }
        
        typename avl_tree::pointer operator->() const
        {
            return node_->value;
        }
        
        bool operator==(const iterator& it) const
        {
            return node_ == it.node_;
        }
        
        bool operator!=(const iterator& it) const
        {
            return node_ != it.node_;
        }
        
    private:
        node* node_;
    };
	typedef iterator avliter;
    class const_iterator
    :public std::iterator<std::bidirectional_iterator_tag, node>
    {
	public:
        
        const_iterator(node* n):node_(n){}
        
        const_iterator():node_(0){}
        
        ~const_iterator(){}
        
        const_iterator(const const_iterator& it)
        {
            node_ = it.node_;
        }
        
        const_iterator& operator=(const avliter& it){
            node_ = it.node_;
            return *this;
        }
        
        const_iterator& operator=(const const_iterator& it)
        {
            node_ = it.node_;
            return *this;
        }
        
        const_iterator& operator++()
        {
            if(node_->right != 0)
            {
                node_ = node_->right;
                while(node_->left != 0)
                    node_ = node_->left;
            }
            else
            {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->left == temp) break;
                }while(node_->parent != 0);
            }
            return *this;
        }
        
        const_iterator operator++(int)
        {
            const_iterator temp1 = *this;
            if(node_->right != 0)
            {
                node_ = node_->right;
                while(node_->left != 0)
                    node_ = node_->left;
            }
            else
            {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->left == temp) break;
                }while(node_->parent != 0);
            }
            return temp1;
        }
        
        const_iterator& operator--() {
            if (node_ -> left != 0){
                node_ = node_->left;
                while(node_->right != 0) node_ = node_->right;
            } else {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->right == temp) break;
                }while(node_->parent != 0 && node_ == node_->parent->left);
            }
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator temp1 = *this;
            if (node_ -> left != 0){
                node_ = node_->left;
                while(node_->right != 0) node_ = node_->right;
            } else {
                node* temp;
                do{
                    temp = node_;
                    node_ = node_->parent;
                    if (node_!=0 && node_->right == temp) break;
                }while(node_->parent != 0 && node_ == node_->parent->left);
            }
            return temp1;
        }
        
        const_reference operator*() const
        {
            return *node_->value;
        }
        
        const_pointer operator->() const
        {
            return node_->value;
        }
        
        bool operator==(const iterator& it) const
        {
            return node_ == it.node_;
        }
        
        bool operator==(const const_iterator& it) const
        {
            return node_ == it.node_;
        }
        
        bool operator!=(const avliter& it) const{
            return node_ != it.node_;
        }
        
        bool operator!=(const const_iterator& it) const{
            return node_ != it.node_;
        }
        
    private:
        node* node_;
    };
public:
    typedef std::reverse_iterator<iterator>             reverse_iterator;
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;
    typedef std::iterator_traits<iterator>              difference_type;
    
private:
    node* root_;
    node* min_node_;
    node* max_node_;
    node* left_barrier;
    node* right_barrier;
    key_compare key_compare_;
    size_type node_count_;
    
public:
	explicit avl_tree(const key_compare& comp = key_compare())
    :key_compare_(comp), node_count_(0)
	{
        initialize();
	}
    
	template <class InputIterator>
	avl_tree(InputIterator first, InputIterator last,
			 const key_compare& comp = key_compare())
    :key_compare_(comp), node_count_(0)
	{
        initialize();
		insert(first, last);
	}
    
	avl_tree(const avl_tree<key, T>& n){
		node_count_ = 0;
		initialize();
		insert(n.begin(), n.end());
	}
    
	template <class InputIterator>
	void erase(InputIterator first, InputIterator last){
		for (; first != last; ++first) erase(*first);
	}
    
	void erase(iterator i){
		node* a = i.node_;
		iterator j = i;
		if (a->left != 0){
			j--;
			node* b = j.node_;
			value_type* temp = a->value;
			a->value = b->value;
			b->value = temp;
			node* parent = b->parent;
			if (parent->left == b) parent->left = 0; else parent->right = 0;
			b->parent = 0;
			delete b;
			rebalance(parent);
		} else if (a->right != 0) {
			j++;
			node* b = j.node_;
			value_type* temp = a->value;
			a->value = b->value;
			b->value = temp;
			node* parent = b->parent;
			if (parent->left == b) parent->left = 0; else parent->right = 0;
			b->parent = 0;
			delete b;
			rebalance(parent);
		} else {
			node* parent = a->parent;
			if (parent->left == a) parent->left = 0; else parent->right = 0;
			a->parent = 0;
			delete a;
			rebalance(parent);
		}
	}
    
	size_type erase(const key_type& k){
		iterator temp = find(k);
		if (temp == end()) return 0;
		else {
			erase(temp);
			return 1;
		}
	}
    
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for(; first != last; ++first)
		{
			insert(*first);
		}
    }
	
	iterator insert(iterator _where, const value_type& val){
		std::pair<iterator, bool> res = insert(val);
		if (res.second) return res.first;
		else return end();
	}
    
    std::pair<iterator, bool>
    insert(const value_type& val)
	{
    	remove_barrier();
    	typedef std::pair<iterator, bool> _Res;
        std::pair<iterator, iterator> __res = get_insert_pos(val.first);
        
        if (__res.first != 0){
        	_Res result = _Res(insert_impl(__res.second.node_, val), true);
        	add_barrier();
        	return result;
        }
        add_barrier();
        return _Res(__res.second, false);
	}
	
	std::pair<iterator, bool>
	emplace(const value_type& val){
		return insert(val);
	}
	
	std::pair<iterator, bool>
	emplace_hint(const_iterator where, const value_type& val){
		return insert(val);
	}
	
	std::pair<iterator, bool>
	emplace_hint(iterator where, const value_type& val){
		return insert(val);
	}
	
    mapped_type&
    operator[](const key_type& k)
    {
        
        iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
        if (i == end() || key_comp()(k, i->first))
            i = (insert(value_type(k, mapped_type()))).first;
        return i->second;
    }
    
    
    ~avl_tree(){
        free_mem(root_);
    }
    
    //
    
    void __print(){
    	root_->__print();
    }
    
    
	bool operator== (const avl_tree& rhs ){
		if (size() != rhs.size()) return false;
		else {
			if (size() == 0) return true;
			iterator i = begin();
			const_iterator j = rhs.begin();
			while (i != end() && j != rhs.end()){
				if ((*i) != (*j)) return false;
				i++; j++;
			}
			return true;
		}
	}
    
	bool operator!= (const avl_tree& rhs ){
		if (size() != rhs.size()) return true;
		else {
			if (size() == 0) return false;
			iterator i = begin();
			const_iterator j = rhs.begin();
			while (i != end() && j != rhs.end()){
				if ((*i) != (*j)) return true;
				i++; j++;
			}
			return false;
		}
	}
    
	bool operator< (const avl_tree& rhs ){
		if (size() == 0) return false;
		iterator i = begin();
		const_iterator j = rhs.begin();
		while (i != end() && j != rhs.end()){
			if ((*i) < (*j)) return true;
			else if ((*i) > (*j)) return false;
			i++; j++;
		}
		if (i == end()){
			if (j == rhs.end()) return false;
			else return true;
		} else return false;
	}
    
	bool operator<= (const avl_tree& rhs ){
		if (size() == 0) return false;
		iterator i = begin();
		const_iterator j = rhs.begin();
		while (i != end() && j != rhs.end()){
			if ((*i) < (*j)) return true;
			else if ((*i) > (*j)) return false;
			i++; j++;
		}
		if (i == end()){
			if (j == rhs.end()) return true;
			else return true;
		} else return false;
	}
    
	bool operator> (const avl_tree& rhs ){
		if (size() == 0) return false;
		iterator i = begin();
		const_iterator j = rhs.begin();
		while (i != end() && j != rhs.end()){
			if ((*i) < (*j)) return false;
			else if ((*i) > (*j)) return true;
			i++; j++;
		}
		if (i == end()){
			if (j == rhs.end()) return false;
			else return false;
		} else return true;
	}
    
	bool operator>= (const avl_tree& rhs ){
		if (size() == 0) return false;
		iterator i = begin();
		const_iterator j = rhs.begin();
		while (i != end() && j != rhs.end()){
			if ((*i) < (*j)) return false;
			else if ((*i) > (*j)) return true;
			i++; j++;
		}
		if (i == end()){
			if (j == rhs.end()) return true;
			else return false;
		} else return true;
	}
    
	avl_tree& operator= (const avl_tree& n){
		clear();
		const_iterator it;
		for (it = n.begin(); it != n.end(); it++){
			insert(*it);
		}
		return *this;
	}
    
    mapped_type at(const key_type &k){
    	iterator res = find(k);
    	if (res == end()) throw std::out_of_range("key doesn't exist");
    	return res.node_->value->second;
    }
    
    
    
	const_iterator begin() const NOEXCEPT
	{
        return const_iterator(min_node_);
	}
    
	iterator begin() NOEXCEPT
    {
        return iterator(min_node_);
    }
    
	const_iterator cbegin() const NOEXCEPT
	{
        return const_iterator(min_node_);
	}
    
    reverse_iterator rbegin() NOEXCEPT
    {
        return reverse_iterator(iterator(max_node_));
    }
    
    const_reverse_iterator rbegin() const NOEXCEPT
    {
        return const_reverse_iterator(iterator(max_node_));
    }
    
    const_reverse_iterator crbegin() const NOEXCEPT
    {
        return const_reverse_iterator(iterator(max_node_));
    }
    
    //
    
    iterator end() NOEXCEPT
    {
        return iterator(right_barrier);
    }
    
    const_iterator end() const NOEXCEPT
    {
        return const_iterator(right_barrier);
    }
    
    const_iterator cend() const NOEXCEPT
    {
        return const_iterator(right_barrier);
    }
    
    reverse_iterator rend() NOEXCEPT
    {
        return reverse_iterator(left_barrier);
    }
    
    const_reverse_iterator rend() const NOEXCEPT
    {
        return const_reverse_iterator(left_barrier);
    }
    
    const_reverse_iterator crend() const NOEXCEPT
    {
        return const_reverse_iterator(left_barrier);
    }
    
    //
    
    bool empty() const NOEXCEPT
    {
        return (node_count_ == 0);
    }
    
    void clear(){
    	node_count_ = 0;
    	free_mem(root_);
    	initialize();
    }
    
    size_type size() const NOEXCEPT
    {
        return node_count_;
    }
    
    iterator find(const key_type& k)
    {
        iterator j = lower_bound(k);
        return (j == end() || key_compare_(k,j->first)) ? end() : j;
    }
    
    const_iterator find(const key_type& k) const
    {
        const_iterator j = lower_bound(k);
		const_iterator e = end();
        return (j == e || key_compare_(k,j->first)) ? e : j;
    }
    
    iterator lower_bound(const key_type& k)
    {
    	if (size() == 0) return end();
        node* x = root_;
        node* y = end().node_;
        while (x != 0 && x != left_barrier && x != right_barrier){
        	if (key_compare_(x->value->first, k)){
        		x = x->right;
        	}
            else{
            	y = x, x = x->left;
            }
        }
        return iterator(y);
    }
    
    const_iterator lower_bound(const key_type& k) const
    {
        if (size() == 0) return end();
        node* x = root_;
        node* y = right_barrier;
        while (x != 0 && x != left_barrier && x != right_barrier){
            if (key_compare_(x->value->first, k)){
                x = x->right;
            }
            else{
                y = x, x = x->left;
            }
        }
        return const_iterator(y);
    }
    
    iterator upper_bound(const key_type& k)
    {
    	if (size() == 0) return end();
        node* x = root_;
        node* y = end().node_;
        while (x != 0 && x != right_barrier && x != left_barrier)
            if (key_compare()(k, x->value->first))
                y = x, x = x->left;
            else
                x = x->right;
        return iterator(y);
    }
    
    const_iterator upper_bound(const key_type& k) const
	{
		if (size() == 0) return end();
		node* x = root_;
		node* y = end().node_;
		while (x != 0 && x != right_barrier && x != left_barrier)
			if (key_compare()(k, x->value->first))
				y = x, x = x->left;
			else
				x = x->right;
		return const_iterator(y);
	}
    
    void swap(avl_tree<key_type, mapped_type>& m){
        node* mroot = m.root_;
        node* mmin_node_ = m.min_node_;
        node* mmax_node_ = m.max_node_;
        node* mleft_barrier = m.left_barrier;
        node* mright_barrier = m.right_barrier;
        size_type mnode_count_ = m.node_count_;
        
        m.root_ = root_;
        m.min_node_ = min_node_;
        m.max_node_ = max_node_;
        m.left_barrier = left_barrier;
        m.right_barrier = right_barrier;
        m.node_count_ = node_count_;
        
        root_ = mroot;
        min_node_ = mmin_node_;
        max_node_ = mmax_node_;
        left_barrier = mleft_barrier;
        right_barrier = mright_barrier;
        node_count_ = mnode_count_;
    }
    
    std::pair<iterator,iterator>
    equal_range(const key_type& k)
    {
        iterator x = begin();
        iterator y = end();
        while (x != 0)
        {
            if (key_compare_(x->first, k))
                x = x.node_->right;
            else if (key_compare_(k, x->first))
                y = x, x = x.node_->left;
            else
            {
                iterator xu(x), yu(y);
                y = x, x = x.node_->left;
                xu = xu.node_->right;
                return std::pair<iterator,iterator>
                (lower_bound(x, y, k), upper_bound(xu, yu, k));
            }
        }
        return std::pair<iterator, iterator>(iterator(y),
                                             iterator(y));
    }
    
    std::pair<const_iterator,const_iterator>
    equal_range(const key_type& k) const
    {
        iterator x = begin();
        iterator y = end();
        while (x != 0)
        {
            if (key_compare_(x->first, k))
                x = x.node_->right;
            else if (key_compare_(k, x->first))
                y = x, x = x.node_->left;
            else
            {
                iterator xu(x), yu(y);
                y = x, x = x.node_->left;
                xu = xu.node_->right;
                return std::pair<iterator,iterator>
                (lower_bound(x, y, k), upper_bound(xu, yu, k));
            }
        }
        return std::pair<iterator, iterator>(iterator(y), iterator(y));
    }
    
	allocator_type get_allocator() const NOEXCEPT
	{
		return allocator_type();
	}
    
	size_type max_size() const NOEXCEPT
	{
		return get_allocator().size();
	}
    
	key_compare key_comp() NOEXCEPT
    {
        return key_compare();
    }
    
	value_compare value_comp() const
	{
		return value_compare(key_compare_());
	}
    
    size_type count(const key_type& k) const
    {
		const_iterator f = find(k);
		const_iterator e = end();
        return f == e ? 0 : 1;
    }
private: // helper functions
    void initialize(){
    	root_ = new node();
    	min_node_ = 0;
    	max_node_ = 0;
    	left_barrier = new node();
    	left_barrier->height = 0;
    	left_barrier->parent = root_;
    	right_barrier = new node();
    	right_barrier->height = 0;
    	right_barrier->parent = root_;
    	root_->left = left_barrier;
    	root_->right = right_barrier;
    	root_->height = 0;
    }
    
    void free_mem(node* node_){
        if (node_ != 0 && node_->left != 0) free_mem(node_->left);
        if (node_ != 0 && node_->right != 0) free_mem(node_->right);
        if (node_ != 0) delete node_;
    }
    
    void remove_barrier(){
    	if (size() > 0){
    		min_node_->left = 0;
    		left_barrier->parent = 0;
    		max_node_->right = 0;
    		right_barrier->parent = 0;
    	}
    }
    
    void add_barrier(){
    	if (size() > 0){
    		min_node_->left = left_barrier;
    		left_barrier->parent = min_node_;
    		max_node_->right = right_barrier;
    		right_barrier->parent = max_node_;
    	}
    }
    
    std::pair<iterator, iterator>
    get_insert_pos(const key_type& k)
    {
    	typedef std::pair<iterator, iterator> __Res;
        if (size()==0) return __Res(iterator(root_), iterator(root_));
        iterator f = find(k);
        if (f != end()) return __Res(0, f);
        iterator x = root_->parent;
        iterator y = root_;
        bool comp = true;
        while (y != 0)
        {
        	x = y;
            comp = key_compare_(k, y.node_->value->first);
            y = comp ? y.node_->left : y.node_->right;
        }
        iterator j = iterator(x);
        return __Res(j, j);
    }
    
    iterator
    insert_impl(node* p, const value_type& v)
    {
    	value_type *value = new value_type(v.first, v.second);
        if (size()==0){
        	root_->value = value;
        	root_->update_balance();
			++node_count_;
			min_node_ = root_;
			max_node_ = root_;
			return iterator(root_);
        } else {
            node* z = new node;
            z->value = value;
            z->height = 1;
            if (p->parent == 0){
        		z->parent = p;
        		if (key_compare_(value->first, p->value->first)) p->left = z;
        		else p->right = z;
        		p->update_balance();
        	} else {
        		bool insert_left = (p == root_
                                    || key_compare_(value->first, p->value->first));
        		z->parent = p;
        		insert_and_rebalance(insert_left, z, p);
        	}
            
			++node_count_;
			if (min_node_ == 0 || min_node_->value->first > z->value->first){
        		min_node_ = z;
			}
            
			if (max_node_ == 0 || max_node_->value->first < z->value->first){
        		max_node_ = z;
			}
		    return iterator(z);
        }
    }
    
    node* right_rotation(node *a){
    	node *parent = a->parent;
    	node *b = a->right;
    	if (b->left_height() < b->right_height()){
            // need single rotation
    		if (root_==a) root_=b;
    		node *t1 = b->left;
            //restructure
    		if (parent != 0){
    			bool is_left_child = ( a == parent->left);
    			if (is_left_child) parent->left = b; else parent->right = b;
    		}
    		b->parent = parent;
    		b->left = a; a->parent = b;
    		a->right = t1; if(t1) t1->parent = a;
    		a->update_balance();
    		b->update_balance();
    		return b;
    	} else {
            // need double rotation
    		node *c = b->left;
    		if (root_==a) root_=c;
    		node *t1 = c->left;
    		node *t2 = c->right;
            // restructure
    		if (parent != 0){
    			bool is_left_child = ( a == parent->left);
    			if (is_left_child) parent->left = c; else parent->right = c;
    		}
			c->parent = parent;
			c->left = a; a->parent = c;
			c->right = b; b->parent = c;
			a->right = t1; if(t1) t1->parent = a;
			b->left = t2; if(t2) t2->parent = b;
			b->update_balance();
			a->update_balance();
			c->update_balance();
			return c;
    	}
    }
    
    node* left_rotation(node* a){
    	node *parent = a->parent;
		node *b = a->left;
		if (b->right_height() < b->left_height()){
            // need single rotation
			if (root_==a) root_=b;
			node* t1 = b->right;
            // restructure
			if (parent != 0){
				bool is_left_child = ( a == parent->left);
				if (is_left_child) parent->left = b; else parent->right = b;
			}
			b->parent = parent;
			b->right = a; a->parent = b;
			a->left = t1; if(t1) t1->parent = a;
			a->update_balance();
			b->update_balance();
			return b;
		} else {
            // need double rotation
			node* c = b->right;
			if (root_==a) root_=c;
			node* t2 = c->left;
			node* t1 = c->right;
			if (parent != 0){
				bool is_left_child = ( a == parent->left);
				if (is_left_child) parent->left = c; else parent->right = c;
			}
			c->parent = parent;
			c->left = b; b->parent = c;
			c->right = a; a->parent = c;
			b->right = t2; if(t2) t2->parent = b;
			a->left = t1; if(t1) t1->parent = a;
			a->update_balance();
			b->update_balance();
			c->update_balance();
			return c;
		}
    }
    
    void rebalance(node *p){
    	node *temp = p;
    	while (temp != 0){
    		temp->update_balance();
    		if (temp->balance < -1){ // need right rotation
    			temp = right_rotation(temp);
    		} else if (temp->balance > 1){ // need left rotation
    			temp = left_rotation(temp);
    		}
    		temp = temp -> parent;
    	}
    }
    
    void insert_left_and_rebalance(node *new_node, node *p){
    	p->left = new_node;
    	rebalance(p);
    }
    
    void insert_right_and_rebalance(node *new_node, node *p){
    	p->right = new_node;
    	rebalance(p);
    }
    
    void insert_and_rebalance(bool insert_left, node *new_node, node *p){
    	if (insert_left){
    		insert_left_and_rebalance(new_node, p);
    	} else {
    		insert_right_and_rebalance(new_node, p);
    	}
    }
    
    iterator lower_bound(iterator first, iterator last, const key_type& k)
    {
        while (first != 0)
            if (key_compare()(first->first, k))
                first = first.node_->right;
            else
                last = first, first = first.node_->left;
        return iterator(last);
    }
    
    iterator upper_bound(iterator first, iterator last, const key_type& k)
    {
        while (first != 0)
            if (key_compare()(k, first->first))
                last = first, first = first.node_->left;
            else
                first = first.node_->right;
        return iterator(last);
    }
};

#endif // AVL_MAP_H
