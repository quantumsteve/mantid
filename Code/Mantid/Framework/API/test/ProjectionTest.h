#ifndef MANTID_API_PROJECTIONTEST_H_
#define MANTID_API_PROJECTIONTEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidAPI/Projection.h"

using namespace Mantid;
using namespace Mantid::API;

class ProjectionTest : public CxxTest::TestSuite {
public:
  void test_blank_constructor() {
    Projection p;
    TS_ASSERT_EQUALS(p.getOffset(0), 0.0);
    TS_ASSERT_EQUALS(p.U(), V3D(1,0,0));
    TS_ASSERT_EQUALS(p.V(), V3D(0,1,0));
    TS_ASSERT_EQUALS(p.W(), V3D(0,0,1));
    TS_ASSERT_EQUALS(p.getUnit(0), RLU);
    TS_ASSERT_EQUALS(p.getUnit(1), RLU);
    TS_ASSERT_EQUALS(p.getUnit(2), RLU);
  }

  void test_uvw_constructors() {
    V3D u(1, -1, 0);
    V3D v(1, 1, 0);
    V3D w(0, 0, 1);
    Projection p(u, v, w);

    TS_ASSERT_EQUALS(p.U(), u);
    TS_ASSERT_EQUALS(p.V(), v);
    TS_ASSERT_EQUALS(p.W(), w);
  }

  void test_throw_out_of_range_access() {
    Projection p;
    TS_ASSERT_THROWS_ANYTHING(p.getOffset(-1));
    TS_ASSERT_THROWS_NOTHING(p.getOffset(2));
    TS_ASSERT_THROWS_ANYTHING(p.getOffset(3));

    TS_ASSERT_THROWS_ANYTHING(p.getAxis(-1));
    TS_ASSERT_THROWS_NOTHING(p.getAxis(2));
    TS_ASSERT_THROWS_ANYTHING(p.getAxis(3));

    TS_ASSERT_THROWS_ANYTHING(p.getUnit(-1));
    TS_ASSERT_THROWS_NOTHING(p.getUnit(2));
    TS_ASSERT_THROWS_ANYTHING(p.getUnit(3));
  }

  void test_copy_constructor() {
    V3D u(1, -1, 0);
    V3D v(1, 1, 0);
    V3D w(0, 0, 1);
    Projection p(u, v, w);
    p.setUnit(0, RLU);
    p.setUnit(1, INV_ANG);

    Projection q(p);

    TS_ASSERT_EQUALS(q.getAxis(0), u);
    TS_ASSERT_EQUALS(q.getAxis(1), v);
    TS_ASSERT_EQUALS(q.getAxis(2), w);
    TS_ASSERT_EQUALS(q.getUnit(0), RLU);
    TS_ASSERT_EQUALS(q.getUnit(1), INV_ANG);
  }

  void test_assign() {
    V3D u(1, -1, 0);
    V3D v(1, 1, 0);
    V3D w(0, 0, 1);
    Projection p(u, v, w);

    Projection q;

    q = p;

    TS_ASSERT_EQUALS(q.getAxis(0), u);
    TS_ASSERT_EQUALS(q.getAxis(1), v);
    TS_ASSERT_EQUALS(q.getAxis(2), w);
  }

  void test_setOffset() {
    Projection p;
    p.setOffset(0, 1.00);
    p.setOffset(1, 1.50);
    p.setOffset(2, 4.75);
    TS_ASSERT_EQUALS(p.getOffset(0), 1.00);
    TS_ASSERT_EQUALS(p.getOffset(1), 1.50);
    TS_ASSERT_EQUALS(p.getOffset(2), 4.75);
  }

  void test_setAxis() {
    Projection p;
    p.setAxis(0, V3D(1,2,3));
    p.setAxis(1, V3D(4,5,6));
    p.setAxis(2, V3D(7,8,8));
    TS_ASSERT_EQUALS(p.getAxis(0), V3D(1,2,3));
    TS_ASSERT_EQUALS(p.getAxis(1), V3D(4,5,6));
    TS_ASSERT_EQUALS(p.getAxis(2), V3D(7,8,8));
  }

  void test_setUnit() {
    Projection p;
    p.setUnit(0, INV_ANG);
    p.setUnit(1, RLU);
    p.setUnit(2, INV_ANG);
    TS_ASSERT_EQUALS(p.getUnit(0), INV_ANG);
    TS_ASSERT_EQUALS(p.getUnit(1), RLU);
    TS_ASSERT_EQUALS(p.getUnit(2), INV_ANG);
  }
};

#endif /* MANTID_API_PROJECTIONTEST_H_ */
