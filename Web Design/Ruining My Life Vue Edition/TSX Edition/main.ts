import { createApp } from 'vue';
import { createRouter, createWebHashHistory } from 'vue-router';
import App from './App.vue';
import LeTsx from './routetsx.vue';
import Cock from './routefirst.vue';
import About from './routesecond.vue';
import Home from './routehome.vue';

const routes = [
    {path: '/', component: Home},
    {path: '/LeTsx', component: LeTsx},
    {path: '/Cock', component: Cock},
    {path: '/About', component: About}
];

const router = createRouter({
    history: createWebHashHistory(),
    routes,
});

createApp(App).use(router).mount('#app')