//
// Created by Herobrine1224 on 2026/7/9.
//

#ifndef STL_VECTOR_H
#define STL_VECTOR_H
#include <cstdint>

namespace RF {
    template<typename T>
    class vector {
    private:
        uint64_t datas_length=0; //元素数量
        uint64_t spatial_length=0; //容量大小
        T* datas = nullptr;
        T default_element=T();

        void relength() {
            if (datas_length==spatial_length) {
                spatial_length=spatial_length==0 ? 1 : spatial_length*2;
                T* new_datas=new T[spatial_length];
                std::copy(datas+0,datas+datas_length,new_datas);
                delete[] datas;
                datas=new_datas;
            }
        }
    public:
        //迭代器封装
        using iterator=T*;
        using const_iterator=const T*;
        iterator begin(){return datas;}
        iterator end(){return datas+datas_length;}
        const_iterator begin() const{return datas;}
        const_iterator end() const{return datas+datas_length;}
        //反向迭代器封装
        using reverse_iterator=std::reverse_iterator<iterator>;
        using const_reverse_iterator=std::reverse_iterator<const_iterator>;
        reverse_iterator rbegin(){return reverse_iterator(end());}
        reverse_iterator rend(){return reverse_iterator(begin());}
        const_reverse_iterator rbegin() const {return const_reverse_iterator(end());}
        const_reverse_iterator rend() const {return const_reverse_iterator(begin());}
        //直接构造
        vector()=default;
        explicit vector(const uint64_t size) {
            datas=new T[size];
            datas_length=size;
            spatial_length=size;
        }
        //初始构造
        explicit vector(const uint64_t size,T element) {
            datas=new T[size];
            for (uint64_t i=0;i<size;++i) {datas[i]=element;}
            datas_length=size;
            spatial_length=size;
        }
        //移动构造
        vector(vector&& other) noexcept {
            this->datas=other.datas;
            this->datas_length=other.datas_length;
            this->spatial_length=other.spatial_length;
            other.datas=nullptr;
            other.datas_length=0;
            other.spatial_length=0;
        }
        //拷贝构造
        vector(const vector& other) {
            spatial_length=other.spatial_length;
            datas_length=other.datas_length;
            datas=new T[spatial_length];
            std::copy(other.datas+0,other.datas+other.datas_length,datas);
        }
        //列表构造
        vector(std::initializer_list<T> init) {
            spatial_length=init.size();
            datas_length=init.size();
            datas=new T[spatial_length];
            uint64_t i=0;
            for (const T elem:init) {
                datas[i++]=elem;
            }
        }
        //拷贝赋值
        vector& operator=(const vector& other) {
            if (this==&other) return *this;
            delete[] datas;
            spatial_length=other.spatial_length;
            datas_length=other.datas_length;
            datas=new T[spatial_length];
            std::copy(other.datas+0,other.datas+datas_length,datas);
            return *this;
        }
        //移动赋值
        vector& operator=(vector&& other) noexcept {
            if (this==&other) return *this;
            delete[] datas;
            datas=other.datas;
            spatial_length=other.spatial_length;
            datas_length=other.datas_length;
            other.datas=nullptr;
            other.spatial_length=0;
            other.datas_length=0;
            return *this;
        }
        //析构函数
        ~vector() {delete[] datas;}
        //下标访问
        [[nodiscard]] const T& operator[](const uint64_t& index) const {
            if (index<datas_length) {return datas[index];}
            return default_element;
        }
        [[nodiscard]] T& operator[](const uint64_t& index) {
            if (index<datas_length) {return datas[index];}
            return default_element;
        }
        //安全访问
        [[nodiscard]] const T& at(const uint64_t index) const {
            if (index>=datas_length) throw std::out_of_range("RF::vector::index out of range");
            return datas[index];
        }
        [[nodiscard]] T& at(const uint64_t index) {
            if (index>=datas_length) throw std::out_of_range("RF::vector::index out of range");
            return datas[index];
        }
        //尾部插入
        void push_back(const T& element) {
            relength();
            datas[datas_length++]=element;
        }
        void push_back(T&& element) {
            relength();
            datas[datas_length++]=std::move(element);
        }
        //尾部构造
        template<typename... Args>
        T& emplace_back(Args&&...args) {
            relength();
            new(&datas[datas_length]) T(std::forward<Args>(args)...);
            return datas[datas_length++];
        }
        //尾部删除
        void pop_back() {
            if (datas_length==0) return;
            datas_length--;
        }
        //重置容量大小
        void reserve(const uint64_t& size) {
            if (size<=spatial_length) return;
            T* new_data=new T[size];
            std::copy(datas+0,datas+datas_length,new_data);
            delete[] datas;
            datas=new_data;
        }
        //尾部元素
        [[nodiscard]] T& back() {
            if (datas_length==0) return default_element;
            return datas[datas_length-1];
        }
        [[nodiscard]]const T& back() const{
            if (datas_length==0) return default_element;
            return datas[datas_length-1];
        }
        //头部元素
        [[nodiscard]]const T& front() const {
            if (datas_length==0) return default_element;
            return datas[0];
        }
        [[nodiscard]] T& front() {
            if (datas_length==0) return default_element;
            return datas[0];
        }
        //插入元素
        void insert(iterator location,const T& element) {
            if (location>end()) return;
            if (location==end()) push_back(element);
            else {
                const uint64_t k=location-begin();
                relength();
                std::copy_backward(datas+k,datas+datas_length,datas+datas_length+1);
                datas[k]=element;
                ++datas_length;
            }
        }
        //构造元素
        template<typename... Arg>
        iterator emplace(iterator loaction,Arg&&...arg) {
            uint64_t k=loaction-begin();
            if (k==datas_length) emplace_back(std::forward<Arg>(arg)...);
            else {
                relength();
                std::copy_backward(datas+k,datas_length,datas+datas_length+1);
                datas[k].~T();
                new(&datas[k]) T(std::forward<Arg>(arg)...);
                ++datas_length;
            }
            return begin()+k;
        }
        void insert(iterator location,T&& element) {
            if (location>end()) return;
            if (location==end()) push_back(element);
            else {
                const uint64_t k=location-begin();
                relength();
                std::copy_backward(datas+k,datas+datas_length,datas+datas_length+1);
                datas[k]=std::move(element);
                ++datas_length;
            }
        }
        //删除元素
        void erase(iterator location) {
            if (location>=end()) return;
            const uint64_t k=location-begin();
            std::copy(datas+k+1,datas+datas_length,datas+k);
            --datas_length;
        }
        //判断非空
        [[nodiscard]] bool empty() const {
            return datas_length==0;
        }
        //返回元素数量
        [[nodiscard]] uint64_t size() const {
            return datas_length;
        }
        //清空元素
        void clear() {
            datas_length=0;
        }
        //重新初始化
        void assign(const uint64_t size,const T& element) {
            spatial_length=size;
            datas_length=spatial_length;
            T* new_datas=new T[spatial_length];
            for (uint64_t i=0;i<datas_length;++i) {
                new_datas[i]=element;
            }
            delete[] datas;
            datas=new_datas;
        }
        //重置数组长度
        void resize(const uint64_t size) {
            if (size>spatial_length) {
                spatial_length=size;
                T* new_datas=new T[spatial_length];
                std::copy(datas+0,datas+datas_length,new_datas);
                for (uint64_t i=datas_length;i<spatial_length;i++) {
                    new_datas[i]=default_element;
                }
                delete[] datas;
                datas=new_datas;
                datas_length=spatial_length;
            }else if (size>datas_length) {
                for (uint64_t i=datas_length;i<size;i++) {
                    datas[i]=default_element;
                }
                datas_length=size;
            }else {
                datas_length=size;
            }
        }
        //返回容量大小
        [[nodiscard]] uint64_t capacity() const {
            return spatial_length;
        }
        //返回数据裸指针
        [[nodiscard]] T* data() {
            return datas;
        }
        [[nodiscard]] const T* data() const {
            return datas;
        }
        //释放多余容量
        void shrink_to_fit() {
            if (datas_length==0) {
                delete[] datas;
                datas=nullptr;
                spatial_length=0;
                return;
            }
            T* new_datas=new T[datas_length];
            spatial_length=datas_length;
            std::copy(datas+0,datas+datas_length,new_datas);
            delete[] datas;
            datas=new_datas;
        }
        //交换向量
        void swap(vector& other) noexcept {
            T* temp_datas=other.datas;
            uint64_t temp_spatial_length=other.spatial_length;
            uint64_t temp_datas_length=other.datas_length;
            other.datas=this->datas;
            other.spatial_length=this->spatial_length;
            other.datas_length=this->datas_length;
            this->datas=temp_datas;
            this->spatial_length=temp_spatial_length;
            this->datas_length=temp_datas_length;
        }
        // 比较运算符
        bool operator==(const vector& other) const {
            if (datas_length!=other.datas_length)  return false;
            for (uint64_t i=0;i<datas_length;i++) {
                if (datas[i]!=other.datas[i]) return false;
            }
            return true;
        }
        bool operator!=(const vector& other) const {
            return !(this==other);
        }
    };
} // RF

#endif //STL_VECTOR_H