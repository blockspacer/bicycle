#pragma once
#include <Eigen/Dense>

namespace controller {
// TODO: Implement linear system interface instead of assuming Bicycle
// TODO: Enforce that linear system discrete time matrices are computed
// TODO: Allow target state to vary in horizon

template<typename T>
class Lqr {
    public:
        using state_t = typename T::state_t;
        using input_t = typename T::input_t;
        using lqr_gain_t = typename Eigen::Matrix<double, T::m, T::n>;
        using state_cost_t = typename T::state_matrix_t;
        using input_cost_t = typename Eigen::Matrix<double, T::m, T::m>;

        //Lqr(T& system, const Q, R, x, xt)
        Lqr(T& system, const state_cost_t& Q, const input_cost_t& R,
                const state_t& xt, uint32_t horizon_iterations) :
            m_system(system), m_horizon(horizon_iterations),
            m_xt(xt), m_Q(Q), m_R(R) { }

        input_t control_calculate(const state_t& x);

        // TODO: maybe just use public member variables?
        void set_horizon(uint32_t horizon_iterations);
        void set_xt(const state_t& xt);
        void set_Q(const state_cost_t& Q);
        void set_R(const input_cost_t& R);

        // accessors
        T& system() const;
        uint32_t horizon_iterations() const;
        state_t xt() const;
        lqr_gain_t K() const;
        state_cost_t P() const;
        state_cost_t Q() const;
        input_cost_t R() const;
        state_t x() const;
        input_t u() const;
        double dt() const;

    private:
        T& m_system;
        uint32_t m_horizon;
        state_t m_x; // starting state
        state_t m_xt; // target state
        input_t m_u; // starting state
        lqr_gain_t m_K;
        state_cost_t m_P; // running horizon cost
        state_cost_t m_Q;
        input_cost_t m_R;

        void update_target();
        void update_lqr_gain();
        void update_horizon_cost();
}; // class Lqr

template<typename T>
inline void Lqr<T>::set_horizon(uint32_t horizon_iterations) {
    m_horizon = horizon_iterations;
}

template<typename T>
inline void Lqr<T>::set_xt(const state_t& xt) {
    m_xt = xt;
}

template<typename T>
inline void Lqr<T>::set_Q(const state_cost_t& Q) {
    m_Q = Q;
}

template<typename T>
inline void Lqr<T>::set_R(const input_cost_t& R) {
    m_R = R;
}

template<typename T>
inline T& Lqr<T>::system() const {
    return m_system;
}

template<typename T>
inline uint32_t Lqr<T>::horizon_iterations() const {
    return m_horizon;
}

template<typename T>
inline typename Lqr<T>::state_t Lqr<T>::xt() const {
    return m_xt;
}

template<typename T>
inline typename Lqr<T>::lqr_gain_t Lqr<T>::K() const {
    return m_K;
}

template<typename T>
inline typename Lqr<T>::state_cost_t Lqr<T>::P() const {
    return m_P;
}

template<typename T>
inline typename Lqr<T>::state_cost_t Lqr<T>::Q() const {
    return m_Q;
}

template<typename T>
inline typename Lqr<T>::input_cost_t Lqr<T>::R() const {
    return m_R;
}

template<typename T>
inline typename Lqr<T>::state_t Lqr<T>::x() const {
    return m_x;
}

template<typename T>
inline typename Lqr<T>::input_t Lqr<T>::u() const {
    return m_u;
}

template<typename T>
inline double Lqr<T>::dt() const {
    return m_system.dt();
}

} // namespace controller

#include "lqr.hh"
