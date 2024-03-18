# Basic CI/CD

Разработка простого **CI/CD** для проекта *SimpleBashUtils*. Сборка, тестирование, развертывание.


## Содержание

1. [Настройка gitlab-runner](#part-1-настройка-gitlab-runner) \
  1.1. [Установка виртуальной машины](#поднять-виртуальную-машину-ubuntu-server-2204-lts) \
  1.2. [Установка gitlab-runner](#скачать-и-установить-на-виртуальную-машину-gitlab-runner) \
  1.3. [Регистрация в gitlab-runner](#запустить-gitlab-runner-и-зарегистрировать-его-для-использования-в-текущем-проекте-do6_cicd)
2. [Cборка](#part-2-сборка) 
  <!-- 2.1. -->

## Part 1. Настройка **gitlab-runner**

**== Задание ==**

### Поднять виртуальную машину *Ubuntu Server 22.04 LTS*

![1.1](screenshots/1.png) 

> В дальнейшем работа на Ubutu Server будет производиться через openssh на декстопной версии Ubuntu

### Скачать и установить на виртуальную машину **gitlab-runner**

> Был выбран метод установки gitlab-runner через [бинарный файл с официального сайта](https://docs.gitlab.com/runner/install/linux-manually.html)

1. Загрузить бинарный файл <br>
```sh
sudo curl -L --output /usr/local/bin/gitlab-runner "https://s3.dualstack.us-east-1.amazonaws.com/gitlab-runner-downloads/latest/binaries/gitlab-runner-linux-amd64"
```
2. Дать файлу разрешение на исполнение: <br>
```sh
sudo chmod +x /usr/local/bin/gitlab-runner
```
3. Создать пользователя GitLab CI <br>
```sh
sudo useradd --comment 'GitLab Runner' --create-home gitlab-runner --shell /bin/bash
```
4. Установить как службу <br>
```sh
sudo gitlab-runner install --user=gitlab-runner --working-directory=/home/gitlab-runner
```
5. Запустить службу
```sh
sudo gitlab-runner start
```

![1.2](screenshots/2.png) 

### Запустить **gitlab-runner** и зарегистрировать его для использования в текущем проекте (*DO6_CICD*)

> Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе

![1.3](screenshots/3.png) 

- Зарегистрировать gitlab-runner
```ssh
sudo gitlab-runner register
```
> Для этого необходимо ввести данные при регистрации: <br>
> 1. Cвой URL-адрес GitLab
> 2. Cвой регистрационный токен
> 3. Описание для у
> 4. Теги для заданий, разделенные запятыми
> 5. Тип исполнителя

![1.4](screenshots/4.png) 


### Part 2. Сборка