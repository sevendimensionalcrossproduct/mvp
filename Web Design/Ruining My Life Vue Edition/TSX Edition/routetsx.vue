<script lang="tsx">
import { defineComponent} from 'vue';

export default defineComponent({
  name: 'MyTsxComponent',
  setup() {
    const handleClick = () => {
      alert('Button clicked!');
    };

    return () => (
      <div>
        <h1>Welcome to My TSX Component</h1>
        <button onClick={handleClick}>Click Me</button>
      </div>
    );
  },
});
</script>

<style scoped>
h1 {
  color: salmon;
}
button {
  font-size: 16px;
}
</style>

