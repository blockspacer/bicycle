#pragma once
#include "bicycle.h"
#include "kalman.h"
#include "lqr.h"
#include "sample_generated.h"

namespace fbs {

// TODO: generate per DiscreteLinear derived class
::fbs::State state(const model::Bicycle::state_t& x) {
    return ::fbs::State(x(0), x(1), x(2), x(3));
}

::fbs::Input input(const model::Bicycle::input_t& u) {
    return ::fbs::Input(u(0), u(1));
}

::fbs::Output output(const model::Bicycle::output_t& y) {
    return ::fbs::Output(y(0), y(1));
}

::fbs::StateMatrix state_matrix(const model::Bicycle::state_matrix_t& a) {
    return ::fbs::StateMatrix(
            a(0, 0), a(0, 1), a(0, 2), a(0, 3),
            a(1, 0), a(1, 1), a(1, 2), a(1, 3),
            a(2, 0), a(2, 1), a(2, 2), a(2, 3),
            a(3, 0), a(3, 1), a(3, 2), a(3, 3));
}

::fbs::InputMatrix input_matrix(const model::Bicycle::input_matrix_t& b) {
    return ::fbs::InputMatrix(
            b(0, 0), b(0, 1),
            b(1, 0), b(1, 1),
            b(2, 0), b(2, 1),
            b(3, 0), b(3, 1));
}

::fbs::OutputMatrix output_matrix(const model::Bicycle::output_matrix_t& c) {
    return ::fbs::OutputMatrix(
            c(0, 0), c(0, 1), c(0, 2), c(0, 3),
            c(1, 0), c(1, 1), c(1, 2), c(1, 3));
}

::fbs::FeedthroughMatrix feedthrough_matrix(const model::Bicycle::feedthrough_matrix_t& d) {
    return ::fbs::FeedthroughMatrix(
            d(0, 0), d(0, 1),
            d(1, 0), d(1, 1));
}

::fbs::SymmetricStateMatrix symmetric_state_matrix(const model::Bicycle::state_matrix_t& m) {
    return ::fbs::SymmetricStateMatrix(
            m(0, 0), m(0, 1), m(0, 2), m(0, 3),
                     m(1, 1), m(1, 2), m(1, 3),
                              m(2, 2), m(2, 3),
                                       m(3, 3));
}

::fbs::SymmetricInputMatrix symmetric_input_matrix(const controller::Lqr<model::Bicycle>::input_cost_t& m) {
    return ::fbs::SymmetricInputMatrix(m(0, 0), m(0, 1), m(1, 1));
}

::fbs::SymmetricOutputMatrix symmetric_output_matrix(
        const observer::Kalman<model::Bicycle>::measurement_noise_covariance_t& m) {
    return ::fbs::SymmetricOutputMatrix(m(0, 0), m(0, 1), m(1, 1));
}

::fbs::SecondOrderMatrix second_order_matrix(const model::Bicycle::second_order_matrix_t& m) {
    return ::fbs::SecondOrderMatrix(m(0, 0), m(0, 1), m(1, 0), m(1, 1));
}

::fbs::KalmanGainMatrix kalman_gain_matrix(const observer::Kalman<model::Bicycle>::kalman_gain_t& k) {
    return ::fbs::KalmanGainMatrix(
            k(0, 0), k(0, 1), k(0, 2), k(0, 3),
            k(1, 0), k(1, 1), k(1, 2), k(1, 3));
}

::fbs::LqrGainMatrix lqr_gain_matrix(const controller::Lqr<model::Bicycle>::lqr_gain_t& k) {
    return ::fbs::LqrGainMatrix(
            k(0, 0), k(0, 1),
            k(0, 2), k(0, 3),
            k(1, 0), k(1, 1),
            k(1, 2), k(1, 3));
}


flatbuffers::Offset<::fbs::Bicycle> create_bicycle(
        flatbuffers::FlatBufferBuilder& fbb,
        const model::Bicycle& bicycle, bool dt = true, bool v = true,
        bool M = true, bool C1 = true, bool K0 = true, bool K2 = true,
        bool Ad = true, bool Bd = true, bool Cd = true, bool Dd = true) {
    double v_ = 0;
    double dt_ = 0;
    auto M_ = second_order_matrix(bicycle.M());
    auto C1_ = second_order_matrix(bicycle.C1());
    auto K0_ = second_order_matrix(bicycle.K0());
    auto K2_ = second_order_matrix(bicycle.K2());
    auto Ad_ = state_matrix(bicycle.Ad());
    auto Bd_ = input_matrix(bicycle.Bd());
    auto Cd_ = output_matrix(bicycle.Cd());
    auto Dd_ = feedthrough_matrix(bicycle.Dd());
    auto Mp = &M_;
    auto C1p = &C1_;
    auto K0p = &K0_;
    auto K2p = &K2_;
    auto Adp = &Ad_;
    auto Bdp = &Bd_;
    auto Cdp = &Cd_;
    auto Ddp = &Dd_;

    if (v) {
        v_ = bicycle.v();
    }
    if (dt) {
        dt_ = bicycle.dt();
    }
    if (!M) {
        Mp = nullptr;
    }
    if (!C1) {
        C1p = nullptr;
    }
    if (!K0) {
        K0p = nullptr;
    }
    if (!K2) {
        K2p = nullptr;
    }
    if (!Ad) {
        Adp = nullptr;
    }
    if (!Bd) {
        Bdp = nullptr;
    }
    if (!Cd) {
        Cdp = nullptr;
    }
    if (!Dd) {
        Ddp = nullptr;
    }
    return CreateBicycle(fbb, v_, dt_, Mp, C1p, K0p, K2p,
            Adp, Bdp, Cdp, Ddp);
}

flatbuffers::Offset<::fbs::Kalman> create_kalman(
        flatbuffers::FlatBufferBuilder& fbb,
        const observer::Kalman<model::Bicycle>& kalman, bool x = true,
        bool P = true, bool Q = true, bool R = true, bool K = true) {
    auto x_ = state(kalman.x());
    auto P_ = symmetric_state_matrix(kalman.P());
    auto Q_ = symmetric_state_matrix(kalman.Q());
    auto R_ = symmetric_output_matrix(kalman.R());
    auto K_ = kalman_gain_matrix(kalman.K());
    auto xp = &x_;
    auto Pp = &P_;
    auto Qp = &Q_;
    auto Rp = &R_;
    auto Kp = &K_;

    if (!x) {
        xp = nullptr;
    }
    if (!P) {
        Pp = nullptr;
    }
    if (!Q) {
        Qp = nullptr;
    }
    if (!R) {
        Rp = nullptr;
    }
    if (!K) {
        Kp = nullptr;
    }
    return CreateKalman(fbb, xp, Pp, Qp, Rp, Kp);
}


flatbuffers::Offset<::fbs::Lqr> create_lqr(
        flatbuffers::FlatBufferBuilder& fbb,
        const controller::Lqr<model::Bicycle>& lqr, bool n = true,
        bool r = true, bool P = true, bool Q = true, bool R = true,
        bool K = true) {
    uint32_t n_ = 0;
    auto r_ = state(lqr.r());
    auto Q_ = symmetric_state_matrix(lqr.Q());
    auto R_ = symmetric_input_matrix(lqr.R());
    auto P_ = symmetric_state_matrix(lqr.P());
    auto K_ = lqr_gain_matrix(lqr.K());
    auto rp = &r_;
    auto Qp = &Q_;
    auto Rp = &R_;
    auto Pp = &P_;
    auto Kp = &K_;

    if (n) {
        n_ = lqr.horizon_iterations();
    }
    if (!r) {
        rp = nullptr;
    }
    if (!Q) {
        Qp = nullptr;
    }
    if (!R) {
        Rp = nullptr;
    }
    if (!P) {
        Pp = nullptr;
    }
    if (!K) {
        Kp = nullptr;
    }
    return CreateLqr(fbb, n_, rp, Qp, Rp, Pp, Kp);
}

} // namespace fbs
