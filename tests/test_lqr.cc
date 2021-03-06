#include "gtest/gtest.h"
#include "test_convergence.h"
#include "constants.h"

// TODO: Add convergence with model error
class LqrTrackingTest: public ConvergenceTest {
    public:
        void SetUp() {
            ConvergenceTest::SetUp();
            // use integral action
            m_lqr->set_Qi((bicycle_t::state_t() <<
                        10.0, 1.0, 1.0, 0.0, 0.0).finished().asDiagonal() *
                    constants::as_radians);
        }
        void simulate(size_t N = default_simulation_length) {
            for(unsigned int i = 0; i < N; ++i) {
                auto u = m_lqr->control_calculate(m_x);
                m_x = m_bicycle->update_state(m_x, u);
            }
        }
};

TEST_P(LqrTrackingTest, ZeroReferenceWithRollTorqueControl) {
    m_lqr->set_R((lqr_t::input_cost_t() <<
            0.1,   0,
              0, 0.1).finished());
    simulate();
    test_state_near(x_true(), bicycle_t::state_t::Zero());
}

TEST_P(LqrTrackingTest, ZeroReference) {
    simulate();
    test_state_near(x_true(), bicycle_t::state_t::Zero());
}

TEST_P(LqrTrackingTest, ZeroReferenceWithPeriodicRollTorqueDisturbance) {
    simulate();

    for(unsigned int i = 0; i < default_simulation_length; ++i) {
        auto u = m_lqr->control_calculate(m_x);

        bool disturb = ((i / 100) % 5 == 0);
        if (disturb) {
            u[0] += 1.0;
        }

        m_x = m_bicycle->update_state(m_x, u);
    }

    test_state_near(x_true(), bicycle_t::state_t::Zero());
}

TEST_P(LqrTrackingTest, ZeroReferenceWithConstantRollTorqueDisturbance) {
    simulate();

    for(unsigned int i = 0; i < default_simulation_length; ++i) {
        auto u = m_lqr->control_calculate(m_x);
        u[0] += 1.0;

        m_x = m_bicycle->update_state(m_x, u);
    }

    test_state_near(x_true(), bicycle_t::state_t::Zero());
}

INSTANTIATE_TEST_CASE_P(
    TrackingRange_1_9,
    LqrTrackingTest,
    ::testing::Range(static_cast<model::real_t>(0.5),
        static_cast<model::real_t>(9.5),
        static_cast<model::real_t>(0.5)));
