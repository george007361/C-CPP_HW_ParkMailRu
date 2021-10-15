#include <gtest/gtest.h>

extern "C" {
#include "computers.h"
#include "menu.h"
}

TEST(list_test_mem, list_init_deinit) {
  List *list;
  EXPECT_TRUE(init_list(&list));

  ASSERT_TRUE(list);
  EXPECT_TRUE(list->head == NULL);
  EXPECT_TRUE(list->tail == NULL);
  EXPECT_EQ(list->count, 0);
  EXPECT_TRUE(deinit_list(&list));
  ASSERT_FALSE(list);
}

class list_test : public ::testing::Test {
 protected:
  List *list;
  PC tmp1;
  PC tmp2;
  PC tmp3;

  virtual void SetUp() {
    init_list(&list);
    PC _tmp1 = {"fst", 1, 11, 111};
    PC _tmp2 = {"snd", 2, 22, 222};
    PC _tmp3 = {"thd", 3, 33, 333};
    tmp1 = _tmp1;
    tmp2 = _tmp2;
    tmp3 = _tmp3;
  }
  virtual void TearDown() { deinit_list(&list); }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(list_test, push_tail_to_empty_list) {
  EXPECT_TRUE(push_tail(list, tmp1));
  EXPECT_TRUE(!strcmp(list->tail->val.name, tmp1.name));
  EXPECT_TRUE(list->head == list->tail);
  EXPECT_TRUE(list->head->prev == NULL);
  EXPECT_TRUE(list->tail->next == NULL);
}

TEST_F(list_test, push_tail_to_not_empty_list) {
  push_tail(list, tmp1);
  int pr_count = list->count;

  EXPECT_TRUE(push_tail(list, tmp2));
  EXPECT_TRUE(!strcmp(list->tail->val.name, tmp2.name));
  EXPECT_TRUE(list->head->prev == NULL);
  EXPECT_TRUE(list->tail->next == NULL);
  EXPECT_EQ(list->count, pr_count + 1);
}

TEST_F(list_test, pop_from_tail_empty_list) {
  PC data;

  EXPECT_FALSE(pop_tail(&data, list));
}

TEST_F(list_test, pop_from_tail_not_empty_list) {
  push_tail(list, tmp1);
  PC data;

  EXPECT_TRUE(pop_tail(&data, list));
  EXPECT_TRUE(!strcmp(data.name, tmp1.name));
}

TEST_F(list_test, get_from_tail_empty_list) {
  Node *tmp = (Node *)malloc(sizeof(Node));
  ASSERT_TRUE(tmp);
  EXPECT_FALSE(get_elem(&tmp, list, 0));

  free(tmp);
}

TEST_F(list_test, get_from_tail_not_empty_list) {
  push_tail(list, tmp1);
  Node *tmp;
  int pr_count = list->count;

  EXPECT_TRUE(get_elem(&tmp, list, 0));
  EXPECT_TRUE(!strcmp(tmp->val.name, tmp1.name));
  EXPECT_TRUE(list->tail->next == NULL);
  EXPECT_EQ(list->count, pr_count);
}

TEST_F(list_test, del_from_tail_empty_list) {
  PC tmp;

  EXPECT_FALSE(get_and_remove(&tmp, list, 0));
}

TEST_F(list_test, del_from_tail_not_empty_list) {
  push_tail(list, tmp1);
  PC tmp;
  int pr_count = list->count;

  EXPECT_TRUE(get_and_remove(&tmp, list, 0));
  EXPECT_TRUE(!strcmp(tmp.name, tmp1.name));
  EXPECT_EQ(list->count, pr_count - 1);
}

TEST_F(list_test, grop_elems_in_empty_list) {
  EXPECT_TRUE(group_computers(&list));
}

TEST_F(list_test, grop_elems_in_not_empty_list) {
  // 1 1 2 3 2 3 1 3 2   Номер в поле name соотв. номеру поля core для удобства
  // отладки
  push_tail(list, tmp1);
  push_tail(list, tmp1);
  push_tail(list, tmp2);
  push_tail(list, tmp3);
  push_tail(list, tmp2);
  push_tail(list, tmp3);
  push_tail(list, tmp1);
  push_tail(list, tmp3);
  push_tail(list, tmp2);

  // exp: 1 1 1 2 2 2 3 3 3
  EXPECT_TRUE(group_computers(&list));

  for (size_t i = 0; i < list->count; i++) {
    Node *tmp;
    get_elem(&tmp, list, i);
    switch (i / 3) {
      case 0:  // первая тройка
      {
        EXPECT_TRUE(!strcmp(tmp->val.name, "fst"));
        break;
      }
      case 1:  // вторая тройка
      {
        EXPECT_TRUE(!strcmp(tmp->val.name, "snd"));
        break;
      }
      case 2:  // третья тройка
      {
        EXPECT_TRUE(!strcmp(tmp->val.name, "thd"));
        break;
      }
      default:
        break;
    }
  }
}

TEST(menu_test, init_free_menu_test) {
  Menu menu = init_menu(1, "pos1");

  ASSERT_TRUE(menu.items);
  EXPECT_TRUE(!strcmp(menu.items[0], "pos1"));
  EXPECT_EQ(menu.count, 1);

  free_menu(&menu);
  ASSERT_FALSE(menu.items);
  EXPECT_EQ(menu.count, 0);
}