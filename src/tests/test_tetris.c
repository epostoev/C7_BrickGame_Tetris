#include "../brick_game/tetris/tetris.h"
#include "../brick_game/brick_game.h"
#include <check.h>


// 1. Tестируем userInput получение сигнала Start
// Ожидаемое поведение (ОП):
// state->fsm == kMove;
// Появление фигуры в поле Next
// Появление фигуры в поле Current
// Статискика установлена в ноль (level == 0, speed == 0, score == 0 )

START_TEST(test_name)
{
  // Arrange
  UserAction_t key_action = Start;
  bool hold = true;
  TetrisState_t *state = getTetrisInfo();
	// Act
  userInput(key_action, hold);
  // Assert
  ck_assert_int_eq(state->fsm, kMove);

}
END_TEST

// 2. Прописать все состояния
// 3. Прописать все сигналы которые должны обрабатываться
// 4. Написать ожидаемое поведение для каждого состояния


// Функция создания набора тестов.
Suite *example_suite_create(void)
{
    Suite *suite = suite_create("Example");
    // Набор разбивается на группы тестов, разделённых по каким-либо критериям.
    TCase *tcase_core = tcase_create("Core of example");
    
    // Добавление теста в группу тестов.
    tcase_add_test(tcase_core, test_name);
    
    // Добавление теста в тестовый набор.
    suite_add_tcase(suite, tcase_core);
    
    return suite;
}

int main(void)
{
    Suite *suite = example_suite_create();
    SRunner *suite_runner = srunner_create(suite);
    
    srunner_run_all(suite_runner, CK_NORMAL);
    // Получаем количество проваленных тестов.
    int failed_count = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    int result_test = EXIT_SUCCESS;
    if (failed_count != 0) {
        // Сигнализируем о том, что тестирование прошло неудачно.
        result_test = EXIT_FAILURE;
    }

    return result_test;
}