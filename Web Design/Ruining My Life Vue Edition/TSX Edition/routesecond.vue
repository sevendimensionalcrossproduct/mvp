<script lang="tsx">
import { defineComponent } from 'vue';
import { RouterLink } from 'vue-router';

export default defineComponent({
  name: 'About',
  setup() {
    const message = 'Hello from about!';

    return () => (
      <div>
        <h2>This is about</h2>
        <p>{message}</p>
        <RouterLink to="/">sdfsdgsdfg</RouterLink>
      </div>
    );
  },
});
</script>

<style scoped>
h2 {
  color: red;
}
</style>
