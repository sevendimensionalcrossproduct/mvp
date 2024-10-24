<script lang="tsx">
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'Cock',
  setup() {
    const message = 'Hello from Cock!';

    return () => (
      <div>
        <h2>This is cock</h2>
        <p>{message}</p>
      </div>
    );
  },
});
</script>

<style scoped>
h2 {
  color: green;
}
</style>
