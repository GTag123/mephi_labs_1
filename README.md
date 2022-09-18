Домашние задачи для курса МИФИ "Введение в программирование, часть 1".

## Клонирование репозитория

```bash
# Переходим в директорию, где вы хотите разместить репозиторий с задачами.
# Например, `/home/<username>/mephi/programming-1` для Linux или `/Users/<username>/mephi/programming-1` для MacOS.
$ cd <выбранная директория>

# Клонируем свой репозиторий, который создается автоматически при регистрации на manytask.
# Имя репозитория доступно по ссылке "MY REPO" на mephi-dap.manytask.org.
$ git clone git@gitlab.manytask.org:mephi-dap-programming-1/students-2022/<ваш репозиторий>

# Переходим в директорию склонированного репозитория.
$ cd <ваш репозиторий>

# Настраиваем свое имя в git.
$ git config --local user.name "<ваш login с mephi-dap.manytask.org>"
$ git config --local user.email "<ваш email с mephi-dap.manytask.org>"

# Настраиваем возможность получать обновления при появлении новых задач.
$ git remote add upstream git@gitlab.manytask.org:mephi-dap-programming-1/public-2022.git
```
